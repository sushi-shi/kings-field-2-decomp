#include "runtime.h"

#include <kf/platform/host.h>
#include <kf/psx/sdk.h>
#include <kf/renderer/renderer.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>

// Graphics library calls. Ordering tables are walked synchronously by DrawOTag;
// each packet becomes a native face drawn in table order into the renderer's
// frame-buffer target. Tag links hold runtime handles instead of addresses.

namespace kf::psx {
namespace {
constexpr u32 tag_terminator = 0xffffff;
constexpr u32 tag_address_mask = 0xffffff;
constexpr std::size_t maximum_packets_per_table = 1u << 20;

struct GpuState {
    DRAWENV draw;
    DISPENV display;
    u16 draw_mode_tpage;
    bool display_changed;
    std::vector<void *> handles;
    std::unordered_map<void *, u32> handle_of;
    std::vector<DrawFace> faces;
    const u8 *tim_cursor;
};
GpuState gpu;

u32 handle_for(void *address) {
    const auto found = gpu.handle_of.find(address);
    if (found != gpu.handle_of.end())
        return found->second;
    const auto handle = static_cast<u32>(gpu.handles.size());
    if (handle >= tag_terminator)
        host_fail("Too many distinct ordering-table packets.");
    gpu.handles.push_back(address);
    gpu.handle_of.emplace(address, handle);
    return handle;
}

u32 &tag_word(void *packet) { return *static_cast<u32 *>(packet); }
void set_link(void *packet, u32 handle) { tag_word(packet) = (tag_word(packet) & ~tag_address_mask) | handle; }

int signed11(u32 value) { return static_cast<int>(value << 21) >> 21; }

BlendMode blend_from_tpage(u16 tpage) {
    switch ((tpage >> 5) & 3) {
    case 0: return BlendMode::average;
    case 1: return BlendMode::add;
    case 2: return BlendMode::subtract;
    default: return BlendMode::add_quarter;
    }
}

DrawArea current_area() {
    return {gpu.draw.clip.x, gpu.draw.clip.y, gpu.draw.clip.w, gpu.draw.clip.h, gpu.draw.dtd != 0};
}

void flush_faces() {
    if (gpu.faces.empty())
        return;
    renderer_draw_faces(host_renderer(), gpu.faces.data(), gpu.faces.size(), current_area());
    gpu.faces.clear();
}

struct PacketReader {
    const u32 *words;
    unsigned index;
    u32 next() { return words[index++]; }
};

void set_position(Vertex &vertex, u32 word) {
    vertex.x = static_cast<float>(signed11(word) + gpu.draw.ofs[0]);
    vertex.y = static_cast<float>(signed11(word >> 16) + gpu.draw.ofs[1]);
}

void set_color(Vertex &vertex, u32 word, bool textured) {
    const float scale = textured ? 128.0f : 255.0f;
    vertex.r = static_cast<float>(word & 0xff) / scale;
    vertex.g = static_cast<float>((word >> 8) & 0xff) / scale;
    vertex.b = static_cast<float>((word >> 16) & 0xff) / scale;
    vertex.a = 1.0f;
}

void set_uv(Vertex &vertex, u32 word) {
    vertex.u = static_cast<float>(word & 0xff) / 256.0f;
    vertex.v = static_cast<float>((word >> 8) & 0xff) / 256.0f;
}

void submit_polygon(const u32 *words) {
    PacketReader reader{words, 0};
    const u32 first = reader.next();
    const u8 code = static_cast<u8>(first >> 24);
    const bool gouraud = code & 0x10, quad = code & 0x08, textured = code & 0x04;
    const bool semi = code & 0x02, raw = code & 0x01;
    const int count = quad ? 4 : 3;
    DrawFace face{};
    face.shape = quad ? FaceShape::Quad : FaceShape::Triangle;
    face.shading = gouraud ? FaceShading::Gouraud : FaceShading::Flat;
    u16 clut = 0, tpage = gpu.draw_mode_tpage;
    for (int i = 0; i < count; ++i) {
        auto &vertex = face.vertices[i];
        set_color(vertex, (i == 0 || !gouraud) ? first : reader.next(), textured);
        set_position(vertex, reader.next());
        if (textured) {
            const u32 word = reader.next();
            set_uv(vertex, word);
            if (i == 0)
                clut = static_cast<u16>(word >> 16);
            if (i == 1)
                tpage = static_cast<u16>(word >> 16);
        }
    }
    if (textured)
        gpu.draw_mode_tpage = tpage;
    face.material.kind = textured ? SurfaceKind::Texture : SurfaceKind::Solid;
    face.material.source = texture_source_from_selectors(tpage, clut);
    face.material.blend = semi ? blend_from_tpage(textured ? tpage : gpu.draw_mode_tpage) : BlendMode::opaque;
    face.material.color_mode = raw ? TextureColorMode::Raw : TextureColorMode::Modulated;
    gpu.faces.push_back(face);
}

void submit_rectangle(const u32 *words) {
    PacketReader reader{words, 0};
    const u32 first = reader.next();
    const u8 code = static_cast<u8>(first >> 24);
    const bool textured = code & 0x04, semi = code & 0x02, raw = code & 0x01;
    const u32 position = reader.next();
    u32 uv = 0;
    if (textured)
        uv = reader.next();
    int width = 1, height = 1;
    switch ((code >> 3) & 3) {
    case 0: {
        const u32 size = reader.next();
        width = size & 0xffff;
        height = size >> 16;
        break;
    }
    case 1: width = height = 1; break;
    case 2: width = height = 8; break;
    default: width = height = 16; break;
    }
    DrawFace face{};
    face.shape = FaceShape::Quad;
    const int x = signed11(position) + gpu.draw.ofs[0];
    const int y = signed11(position >> 16) + gpu.draw.ofs[1];
    const int u = uv & 0xff, v = (uv >> 8) & 0xff;
    for (int i = 0; i < 4; ++i) {
        auto &vertex = face.vertices[i];
        const int dx = (i & 1) ? width : 0, dy = (i & 2) ? height : 0;
        vertex.x = static_cast<float>(x + dx);
        vertex.y = static_cast<float>(y + dy);
        vertex.u = static_cast<float>(u + dx) / 256.0f;
        vertex.v = static_cast<float>(v + dy) / 256.0f;
        set_color(vertex, first, textured);
    }
    face.material.kind = textured ? SurfaceKind::Texture : SurfaceKind::Solid;
    face.material.source = texture_source_from_selectors(gpu.draw_mode_tpage, static_cast<u16>(uv >> 16));
    face.material.blend = semi ? blend_from_tpage(gpu.draw_mode_tpage) : BlendMode::opaque;
    face.material.color_mode = raw ? TextureColorMode::Raw : TextureColorMode::Modulated;
    gpu.faces.push_back(face);
}

void submit_environment(u32 word) {
    switch (word >> 24) {
    case 0xe1:
        gpu.draw_mode_tpage = static_cast<u16>(word & 0x1ff);
        gpu.draw.dtd = (word >> 9) & 1;
        break;
    case 0xe3:
    case 0xe4:
    case 0xe5:
        warn_once("drawing-area packets");
        break;
    default:
        break;
    }
}

void submit_packet(void *packet) {
    const u32 length = tag_word(packet) >> 24;
    if (length == 0)
        return;
    const u32 *words = static_cast<const u32 *>(packet) + 1;
    for (u32 offset = 0; offset < length;) {
        const u8 code = static_cast<u8>(words[offset] >> 24);
        u32 size = 1;
        if (code >= 0x20 && code < 0x40) {
            const bool gouraud = code & 0x10, quad = code & 0x08, textured = code & 0x04;
            const u32 vertices = quad ? 4 : 3;
            size = 1 + vertices * (1 + (textured ? 1 : 0)) + (gouraud ? vertices - 1 : 0);
            if (offset + size <= length)
                submit_polygon(words + offset);
        } else if (code >= 0x60 && code < 0x80) {
            const bool textured = code & 0x04;
            size = 2 + (textured ? 1 : 0) + (((code >> 3) & 3) == 0 ? 1 : 0);
            if (offset + size <= length)
                submit_rectangle(words + offset);
        } else if (code >= 0x40 && code < 0x60) {
            warn_once("line packets");
            return;
        } else if (code >= 0xe1 && code <= 0xe6) {
            submit_environment(words[offset]);
        } else if (code == 0x02) {
            flush_faces();
            const u32 position = words[offset + 1], size_word = words[offset + 2];
            renderer_fill(host_renderer(), position & 0x3f0, (position >> 16) & 0x1ff,
                          ((size_word & 0x3ff) + 15) & ~15, (size_word >> 16) & 0x1ff, words[offset] & 0xff,
                          (words[offset] >> 8) & 0xff, (words[offset] >> 16) & 0xff);
            size = 3;
        } else if (code != 0) {
            warn_once("unknown GPU packets");
            return;
        }
        offset += size;
    }
}
} // namespace

// KF_CAPTURE=DIRECTORY writes every sixtieth presented display as a PPM image.
void capture_display() {
    static const char *directory = std::getenv("KF_CAPTURE");
    static unsigned presented;
    if (!directory || presented++ % 60 != 0)
        return;
    const auto &display = host_renderer()->display;
    if (display.width <= 0 || display.height <= 0)
        return;
    std::vector<u16> words(std::size_t(display.width) * display.height);
    renderer_read_vram(host_renderer(), display.x, display.y, display.width, display.height, words.data());
    char path[512];
    std::snprintf(path, sizeof path, "%s/frame%05u.ppm", directory, presented / 60);
    if (std::FILE *file = std::fopen(path, "wb")) {
        std::fprintf(file, "P6\n%d %d\n255\n", display.width, display.height);
        for (const auto word : words) {
            const u8 rgb[3] = {static_cast<u8>((word & 31) << 3), static_cast<u8>(((word >> 5) & 31) << 3),
                               static_cast<u8>(((word >> 10) & 31) << 3)};
            std::fwrite(rgb, 1, 3, file);
        }
        std::fclose(file);
    }
}

void gpu_present() {
    if (!gpu.display_changed) {
        host_present();
        capture_display();
    }
    gpu.display_changed = false;
}
} // namespace kf::psx

using namespace kf;
using namespace kf::psx;

extern "C" {
int ResetGraph(int) {
    return 0;
}

int SetGraphDebug(int level) { return level; }
void SetDispMask(int) {}
int DrawSync(int) { return 0; }

DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h) {
    std::memset(env, 0, sizeof *env);
    env->clip = {static_cast<short>(x), static_cast<short>(y), static_cast<short>(w), static_cast<short>(h)};
    env->ofs[0] = static_cast<short>(x);
    env->ofs[1] = static_cast<short>(y);
    env->tpage = static_cast<u_short>(getTPage(0, 0, 640, 0));
    env->dtd = 1;
    env->dfe = 1;
    return env;
}

DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h) {
    std::memset(env, 0, sizeof *env);
    env->disp = {static_cast<short>(x), static_cast<short>(y), static_cast<short>(w), static_cast<short>(h)};
    return env;
}

DRAWENV *PutDrawEnv(DRAWENV *env) {
    flush_faces();
    gpu.draw = *env;
    gpu.draw_mode_tpage = env->tpage;
    if (env->isbg)
        renderer_fill(host_renderer(), env->clip.x, env->clip.y, env->clip.w, env->clip.h, env->r0, env->g0, env->b0);
    return env;
}

DISPENV *PutDispEnv(DISPENV *env) {
    gpu.display = *env;
    renderer_set_display(host_renderer(), {env->disp.x, env->disp.y, env->disp.w, env->disp.h});
    host_present();
    capture_display();
    gpu.display_changed = true;
    return env;
}

u_long *ClearOTag(u_long *ot, int n) {
    for (int i = 0; i < n; ++i)
        ot[i] = i + 1 < n ? handle_for(&ot[i + 1]) : tag_terminator;
    return ot;
}

u_long *ClearOTagR(u_long *ot, int n) {
    for (int i = 0; i < n; ++i)
        ot[i] = i > 0 ? handle_for(&ot[i - 1]) : tag_terminator;
    return ot;
}

void AddPrim(void *ot, void *p) {
    set_link(p, tag_word(ot) & tag_address_mask);
    set_link(ot, handle_for(p));
}

void DrawOTag(u_long *p) {
    void *packet = p;
    for (std::size_t steps = 0; packet && steps < maximum_packets_per_table; ++steps) {
        submit_packet(packet);
        const u32 next = tag_word(packet) & tag_address_mask;
        if (next == tag_terminator)
            break;
        if (next >= gpu.handles.size())
            host_fail("Ordering table link does not name a registered packet.");
        packet = gpu.handles[next];
    }
    flush_faces();
}

int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {
    flush_faces();
    renderer_fill(host_renderer(), rect->x, rect->y, rect->w, rect->h, r, g, b);
    return 0;
}

int LoadImage(RECT *rect, u_long *p) {
    flush_faces();
    renderer_write_vram(host_renderer(), rect->x, rect->y, rect->w, rect->h, reinterpret_cast<const u16 *>(p));
    return 0;
}

int StoreImage(RECT *rect, u_long *p) {
    flush_faces();
    renderer_read_vram(host_renderer(), rect->x, rect->y, rect->w, rect->h, reinterpret_cast<u16 *>(p));
    return 0;
}

int MoveImage(RECT *rect, int x, int y) {
    flush_faces();
    std::vector<u16> words(std::size_t(rect->w > 0 ? rect->w : 0) * (rect->h > 0 ? rect->h : 0));
    if (words.empty())
        return 0;
    renderer_read_vram(host_renderer(), rect->x, rect->y, rect->w, rect->h, words.data());
    renderer_write_vram(host_renderer(), x, y, rect->w, rect->h, words.data());
    return 0;
}

int OpenTIM(u_long *addr) {
    gpu.tim_cursor = reinterpret_cast<const u8 *>(addr);
    return 0;
}

// Reads the next TIM image at the cursor. Rectangles and pixels stay in place.
TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg) {
    auto *words = reinterpret_cast<u_long *>(const_cast<u8 *>(gpu.tim_cursor));
    if (!words || (words[0] & 0xff) != 0x10)
        return nullptr;
    timimg->mode = words[1];
    auto *cursor = reinterpret_cast<u8 *>(words + 2);
    timimg->crect = nullptr;
    timimg->caddr = nullptr;
    if (timimg->mode & 8) {
        const u32 bytes = *reinterpret_cast<u32 *>(cursor);
        timimg->crect = reinterpret_cast<RECT *>(cursor + 4);
        timimg->caddr = reinterpret_cast<u_long *>(cursor + 12);
        cursor += bytes;
    }
    const u32 bytes = *reinterpret_cast<u32 *>(cursor);
    timimg->prect = reinterpret_cast<RECT *>(cursor + 4);
    timimg->paddr = reinterpret_cast<u_long *>(cursor + 12);
    gpu.tim_cursor = cursor + bytes;
    return timimg;
}

u_short GetTPage(int tp, int abr, int x, int y) {
    return static_cast<u_short>(getTPage(tp, abr, x, y));
}

u_short GetClut(int x, int y) {
    return static_cast<u_short>(getClut(x, y));
}

void SetPolyFT4(POLY_FT4 *p) {
    setPolyFT4(p);
}

void SetSemiTrans(void *p, int abe) {
    setSemiTrans(p, abe);
}
}
