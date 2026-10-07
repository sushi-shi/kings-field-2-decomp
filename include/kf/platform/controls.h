#ifndef KF_PLATFORM_CONTROLS_H
#define KF_PLATFORM_CONTROLS_H

// Collect platform controls; the original game still owns their gameplay meaning.
#include <kf/platform/types.h>

#include <array>
#include <cstddef>

namespace kf {
inline constexpr unsigned input_binding_capacity = 96;
enum class Action : u8 {
    // One action per PlayStation pad button; the original game assigns meaning.
    pad_up,
    pad_down,
    pad_left,
    pad_right,
    pad_triangle,
    pad_circle,
    pad_cross,
    pad_square,
    pad_l1,
    pad_l2,
    pad_r1,
    pad_r2,
    pad_start,
    pad_select,
    count
};
enum class InputDevice : u8 { keyboard, mouse, gamepad };
struct Control {
    InputDevice device;
    u32 code;
};
struct InputFrame {
    u32 held, pressed, released;
    double look_x, look_y;
};
struct InputBinding {
    Control control;
    Action action;
    bool down;
};
struct InputState {
    std::array<InputBinding, input_binding_capacity> bindings;
    std::size_t binding_count;
    InputFrame pending;
};

u32 action_bit(Action action);
bool input_pressed(const InputFrame *frame, Action action);
bool input_bind(InputState *input, Control control, Action action);
void input_button(InputState *input, Control control, bool down);
void input_motion(InputState *input, double x, double y);
void input_disconnect(InputState *input, InputDevice device);
void input_clear(InputState *input);
InputFrame input_take(InputState *input);
} // namespace kf

#endif // KF_PLATFORM_CONTROLS_H
