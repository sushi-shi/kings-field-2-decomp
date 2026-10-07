#ifndef KF_RENDERER_TEXTURES_H
#define KF_RENDERER_TEXTURES_H

#include <kf/platform/types.h>

#include <vector>

namespace kf {
enum class TextureFormat : u8 { Indexed4, Indexed8, Direct16 };
struct TextureSource {
    u16 x, y, palette_x, palette_y;
    TextureFormat format;
};
struct TextureEntry {
    TextureSource source;
    u32 texture;
    bool dirty;
};
struct TextureStore {
    // CPU mirror of frame-buffer texels and palettes used for texture decoding.
    std::vector<u16> words;
    std::vector<TextureEntry> entries;
};
struct Image;
bool texture_store_write(TextureStore *store, int x, int y, int width, int height, const u16 *words);
bool texture_decode(Image *image, TextureSource source, const u16 *words, std::size_t count);
u32 texture_store_resolve(TextureStore *store, TextureSource source);
void texture_store_release(TextureStore *store);
// Decodes a packed texture-page and palette selector pair.
TextureSource texture_source_from_selectors(u16 tpage, u16 clut);
}

#endif // KF_RENDERER_TEXTURES_H
