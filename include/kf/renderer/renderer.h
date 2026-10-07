#ifndef KF_RENDERER_RENDERER_H
#define KF_RENDERER_RENDERER_H

#include <kf/platform/types.h>
#include <kf/renderer/textures.h>

#include <array>
#include <cstddef>
#include <vector>

namespace kf {
inline constexpr int vram_width = 1024;
inline constexpr int vram_height = 512;

using TextureId = u32;
struct Image {
    u32 width, height;
    std::vector<u8> rgba;
};
// Positions are frame-buffer pixels after the drawing offset is applied.
struct Vertex {
    float x, y, u, v;
    float r, g, b, a;
};
// Values are shared with the pixel shader's blend-mode uniform.
enum class BlendMode : u8 { opaque = 0, average = 1, add = 2, subtract = 3, add_quarter = 4 };
enum class FaceShape : u8 { Triangle = 3, Quad = 4 };
enum class SurfaceKind : u8 { Solid, Texture };
enum class FaceShading : u8 { Flat, Gouraud };
enum class TextureColorMode : u8 { Modulated, Raw };
struct FaceMaterial {
    SurfaceKind kind;
    TextureSource source;
    BlendMode blend;
    TextureColorMode color_mode = TextureColorMode::Modulated;
};
struct DrawFace {
    std::array<Vertex, 4> vertices;
    FaceShape shape;
    FaceMaterial material;
    FaceShading shading = FaceShading::Flat;
};
// Inclusive-exclusive frame-buffer rectangle that drawing may modify.
struct DrawArea {
    int x, y, width, height;
    bool dither;
};
struct DisplayArea {
    int x, y, width, height;
};
// The renderer owns one frame-buffer-sized color target. Texture decoding reads
// the CPU texel mirror in TextureStore, which image transfers keep current.
struct Renderer {
    u32 program, vao, buffer, framebuffer, color_texture, blend_texture;
    TextureId white_texture;
    TextureStore textures;
    DisplayArea display;
};
bool renderer_init(Renderer *renderer, char *error, std::size_t error_size);
void renderer_release(Renderer *renderer);
TextureId renderer_upload(const Image *image);
void renderer_delete_texture(TextureId texture);
// Draws whole faces in submission order; the caller resolves ordering.
bool renderer_draw_faces(Renderer *renderer, const DrawFace *faces, std::size_t count, const DrawArea &area);
void renderer_fill(Renderer *renderer, int x, int y, int width, int height, u8 red, u8 green, u8 blue);
// 15-bit frame-buffer words, row-major, with the semi-transparency bit in bit 15.
void renderer_write_vram(Renderer *renderer, int x, int y, int width, int height, const u16 *words);
void renderer_read_vram(Renderer *renderer, int x, int y, int width, int height, u16 *words);
void renderer_set_display(Renderer *renderer, DisplayArea display);
void renderer_present(const Renderer *renderer, int width, int height);
} // namespace kf

#endif // KF_RENDERER_RENDERER_H
