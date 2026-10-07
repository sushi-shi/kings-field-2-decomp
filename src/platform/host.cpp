#include <kf/audio/sound.h>
#include <kf/platform/controls.h>
#include <kf/platform/host.h>
#include <kf/renderer/renderer.h>

#include <SDL3/SDL.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace kf {
namespace {
struct HostState {
    SDL_Window *window;
    SDL_GLContext context;
    SDL_Gamepad *gamepad;
    Renderer renderer;
    InputState input;
    std::array<SDL_Keycode, SDL_SCANCODE_COUNT> pressed_keys;
    Uint64 epoch, paused_ns, pause_start;
    bool focused;
};
HostState host;
constexpr Uint64 ns_per_second = 1000000000;
constexpr Uint64 ns_per_millisecond = 1000000;
constexpr Uint64 clock_ticks_per_second = 60;
constexpr Uint64 unfocused_poll_interval_ns = 16 * ns_per_millisecond;
constexpr Uint64 maximum_wait_slice_ns = 8 * ns_per_millisecond;
constexpr u32 gamepad_axis_control_first = 100;
constexpr u32 gamepad_axis_positive_first = gamepad_axis_control_first + 1;
constexpr int gamepad_axis_deadzone = 16000;
constexpr std::size_t renderer_error_capacity = 2048;

void platform_yield(Uint64 nanoseconds) {
#ifdef __EMSCRIPTEN__
    emscripten_sleep(static_cast<unsigned>((nanoseconds + ns_per_millisecond - 1) / ns_per_millisecond));
#else
    SDL_DelayNS(nanoseconds);
#endif
}

void bind_inputs() {
    auto *input = &host.input;
    // A direct PlayStation pad layout for bring-up. The game decides what each
    // button means; gameplay-specific bindings belong to the input milestone.
    struct KeyBinding { SDL_Keycode code; Action action; };
    constexpr std::array<KeyBinding, 20> keys = {
        KeyBinding{SDLK_UP, Action::pad_up}, KeyBinding{SDLK_DOWN, Action::pad_down},
        KeyBinding{SDLK_LEFT, Action::pad_left}, KeyBinding{SDLK_RIGHT, Action::pad_right},
        KeyBinding{SDLK_W, Action::pad_up}, KeyBinding{SDLK_S, Action::pad_down},
        KeyBinding{SDLK_A, Action::pad_left}, KeyBinding{SDLK_D, Action::pad_right},
        KeyBinding{SDLK_I, Action::pad_triangle}, KeyBinding{SDLK_L, Action::pad_circle},
        KeyBinding{SDLK_K, Action::pad_cross}, KeyBinding{SDLK_J, Action::pad_square},
        KeyBinding{SDLK_Q, Action::pad_l1}, KeyBinding{SDLK_E, Action::pad_r1},
        KeyBinding{SDLK_1, Action::pad_l2}, KeyBinding{SDLK_3, Action::pad_r2},
        KeyBinding{SDLK_RETURN, Action::pad_start}, KeyBinding{SDLK_SPACE, Action::pad_cross},
        KeyBinding{SDLK_BACKSPACE, Action::pad_select}, KeyBinding{SDLK_ESCAPE, Action::pad_triangle},
    };
    for (const auto &key : keys)
        input_bind(input, {InputDevice::keyboard, static_cast<u32>(key.code)}, key.action);
    struct ButtonBinding { SDL_GamepadButton code; Action action; };
    constexpr std::array<ButtonBinding, 14> buttons = {
        ButtonBinding{SDL_GAMEPAD_BUTTON_DPAD_UP, Action::pad_up},
        ButtonBinding{SDL_GAMEPAD_BUTTON_DPAD_DOWN, Action::pad_down},
        ButtonBinding{SDL_GAMEPAD_BUTTON_DPAD_LEFT, Action::pad_left},
        ButtonBinding{SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Action::pad_right},
        ButtonBinding{SDL_GAMEPAD_BUTTON_NORTH, Action::pad_triangle},
        ButtonBinding{SDL_GAMEPAD_BUTTON_EAST, Action::pad_circle},
        ButtonBinding{SDL_GAMEPAD_BUTTON_SOUTH, Action::pad_cross},
        ButtonBinding{SDL_GAMEPAD_BUTTON_WEST, Action::pad_square},
        ButtonBinding{SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Action::pad_l1},
        ButtonBinding{SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Action::pad_r1},
        ButtonBinding{SDL_GAMEPAD_BUTTON_START, Action::pad_start},
        ButtonBinding{SDL_GAMEPAD_BUTTON_BACK, Action::pad_select},
        ButtonBinding{SDL_GAMEPAD_BUTTON_LEFT_STICK, Action::pad_l2},
        ButtonBinding{SDL_GAMEPAD_BUTTON_RIGHT_STICK, Action::pad_r2},
    };
    for (const auto &button : buttons)
        input_bind(input, {InputDevice::gamepad, static_cast<u32>(button.code)}, button.action);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_control_first + SDL_GAMEPAD_AXIS_LEFTX * 2}, Action::pad_left);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_positive_first + SDL_GAMEPAD_AXIS_LEFTX * 2}, Action::pad_right);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_control_first + SDL_GAMEPAD_AXIS_LEFTY * 2}, Action::pad_up);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_positive_first + SDL_GAMEPAD_AXIS_LEFTY * 2}, Action::pad_down);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_positive_first + SDL_GAMEPAD_AXIS_LEFT_TRIGGER * 2}, Action::pad_l2);
    input_bind(input, {InputDevice::gamepad, gamepad_axis_positive_first + SDL_GAMEPAD_AXIS_RIGHT_TRIGGER * 2}, Action::pad_r2);
}

void open_available_gamepad() {
    int count = 0;
    auto *ids = SDL_GetGamepads(&count);
    for (int i = 0; i < count && !host.gamepad; ++i)
        host.gamepad = SDL_OpenGamepad(ids[i]);
    SDL_free(ids);
}

void process_keyboard_event(const SDL_KeyboardEvent &event) {
    if (event.repeat || event.scancode <= SDL_SCANCODE_UNKNOWN || event.scancode >= SDL_SCANCODE_COUNT)
        return;
    auto &pressed_key = host.pressed_keys[event.scancode];
    if (event.down) {
        if (pressed_key != SDLK_UNKNOWN)
            return;
        // Letter keys follow physical QWERTY positions in every layout.
        if (event.scancode >= SDL_SCANCODE_A && event.scancode <= SDL_SCANCODE_Z)
            pressed_key = SDLK_A + static_cast<SDL_Keycode>(event.scancode - SDL_SCANCODE_A);
        else
            pressed_key = SDL_GetKeyFromScancode(event.scancode, SDL_KMOD_NONE, false);
        if (pressed_key != SDLK_UNKNOWN)
            input_button(&host.input, {InputDevice::keyboard, pressed_key}, true);
    } else {
        const auto released_key = pressed_key;
        pressed_key = SDLK_UNKNOWN;
        if (released_key == SDLK_UNKNOWN)
            return;
        for (const auto held_key : host.pressed_keys)
            if (held_key == released_key)
                return;
        input_button(&host.input, {InputDevice::keyboard, released_key}, false);
    }
}

void process_event(const SDL_Event &event) {
    static const bool trace = std::getenv("KF_TRACE") != nullptr;
    if (trace && (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ||
                  event.type == SDL_EVENT_GAMEPAD_ADDED))
        std::fprintf(stderr, "kf2: input event %u key %d button %d\n", event.type,
                     event.type == SDL_EVENT_KEY_DOWN ? static_cast<int>(event.key.scancode) : -1,
                     event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ? static_cast<int>(event.gbutton.button) : -1);
    switch (event.type) {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        host_shutdown();
        std::exit(0);
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        if (host.focused)
            process_keyboard_event(event.key);
        break;
    case SDL_EVENT_GAMEPAD_ADDED:
        if (!host.gamepad)
            host.gamepad = SDL_OpenGamepad(event.gdevice.which);
        break;
    case SDL_EVENT_GAMEPAD_REMOVED:
        if (host.gamepad && event.gdevice.which == SDL_GetGamepadID(host.gamepad)) {
            SDL_CloseGamepad(host.gamepad);
            host.gamepad = nullptr;
            input_disconnect(&host.input, InputDevice::gamepad);
            open_available_gamepad();
        }
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        if (host.focused && host.gamepad && event.gbutton.which == SDL_GetGamepadID(host.gamepad))
            input_button(&host.input, {InputDevice::gamepad, event.gbutton.button}, event.gbutton.down);
        break;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        if (host.focused && host.gamepad && event.gaxis.which == SDL_GetGamepadID(host.gamepad)) {
            input_button(&host.input, {InputDevice::gamepad, gamepad_axis_control_first + event.gaxis.axis * 2u},
                         event.gaxis.value < -gamepad_axis_deadzone);
            input_button(&host.input, {InputDevice::gamepad, gamepad_axis_positive_first + event.gaxis.axis * 2u},
                         event.gaxis.value > gamepad_axis_deadzone);
        }
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        if (host.focused) {
            host.pause_start = SDL_GetTicksNS();
            host.focused = false;
            sound_set_paused(true);
            input_clear(&host.input);
            host.pressed_keys.fill(SDLK_UNKNOWN);
        }
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        if (!host.focused) {
            host.paused_ns += SDL_GetTicksNS() - host.pause_start;
            host.focused = true;
            sound_set_paused(false);
        }
        break;
    case SDL_EVENT_WINDOW_EXPOSED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        if (event.window.windowID == SDL_GetWindowID(host.window))
            host_present();
        break;
    default:
        break;
    }
}

void wait_until_ns(std::uint64_t target) {
    for (;;) {
        host_poll();
        const auto now = host_clock_ns();
        if (now >= target)
            return;
        platform_yield(std::min<Uint64>(target - now, maximum_wait_slice_ns));
    }
}
} // namespace

bool host_start(const char *title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO)) {
        std::fprintf(stderr, "%s\n", SDL_GetError());
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    host.window = SDL_CreateWindow(title, 960, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (host.window)
        host.context = SDL_GL_CreateContext(host.window);
    if (!host.window || !host.context) {
        std::fprintf(stderr, "%s\n", SDL_GetError());
        host_shutdown();
        return false;
    }
    // Pacing uses the absolute clock, not a second swap-interval wait.
    SDL_GL_SetSwapInterval(0);
    std::array<char, renderer_error_capacity> error{};
    if (!renderer_init(&host.renderer, error.data(), error.size())) {
        std::fprintf(stderr, "%s\n", error.data());
        host_shutdown();
        return false;
    }
    bind_inputs();
    open_available_gamepad();
    host.epoch = SDL_GetTicksNS();
    host.focused = true;
    if (!sound_start()) {
        host_shutdown();
        return false;
    }
    return true;
}

void host_shutdown() {
    sound_shutdown();
    if (host.gamepad)
        SDL_CloseGamepad(host.gamepad);
    if (host.context) {
        renderer_release(&host.renderer);
        SDL_GL_DestroyContext(host.context);
    }
    if (host.window)
        SDL_DestroyWindow(host.window);
    host = {};
    SDL_Quit();
}

[[noreturn]] void host_fail(const char *message) {
    std::fprintf(stderr, "%s\n", message);
    std::fflush(stderr);
    if (host.window)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "King's Field II", message, host.window);
    host_shutdown();
    std::exit(1);
}

void host_poll() {
    for (;;) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            process_event(event);
        sound_poll();
        if (host.focused)
            return;
        platform_yield(unfocused_poll_interval_ns);
    }
}

std::uint64_t host_clock_ns() {
    const auto now = host.focused ? SDL_GetTicksNS() : host.pause_start;
    return now - host.epoch - host.paused_ns;
}

std::uint64_t host_clock_tick() {
    const auto elapsed = host_clock_ns();
    return elapsed / ns_per_second * clock_ticks_per_second +
           elapsed % ns_per_second * clock_ticks_per_second / ns_per_second;
}

void host_wait_until_tick(std::uint64_t deadline) {
    const auto target = deadline / clock_ticks_per_second * ns_per_second +
        (deadline % clock_ticks_per_second * ns_per_second + clock_ticks_per_second - 1) / clock_ticks_per_second;
    wait_until_ns(target);
}

void host_wait_frame() {
    host_wait_until_tick(host_clock_tick() + 1);
}

u32 host_read_pad() {
    host_poll();
    const u32 actions = host.input.pending.held;
    host.input.pending.pressed = host.input.pending.released = 0;
    // Documented digital pad bit positions (active-high after the library's inversion).
    constexpr std::array<u32, static_cast<std::size_t>(Action::count)> bits = {
        1u << 12, 1u << 14, 1u << 15, 1u << 13, // up, down, left, right
        1u << 4, 1u << 5, 1u << 6, 1u << 7,     // triangle, circle, cross, square
        1u << 2, 1u << 0, 1u << 3, 1u << 1,     // L1, L2, R1, R2
        1u << 11, 1u << 8,                      // start, select
    };
    u32 buttons = 0;
    for (std::size_t i = 0; i < bits.size(); ++i)
        if (actions & action_bit(static_cast<Action>(i)))
            buttons |= bits[i];
    return buttons;
}

Renderer *host_renderer() { return &host.renderer; }

void host_present() {
    if (!host.window)
        return;
    int width = 0, height = 0;
    SDL_GetWindowSizeInPixels(host.window, &width, &height);
    if (width > 0 && height > 0) {
        renderer_present(&host.renderer, width, height);
        SDL_GL_SwapWindow(host.window);
    }
}
} // namespace kf
