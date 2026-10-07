#include <kf/renderer/renderer.h>
#include <kf/renderer/textures.h>

#include <new>
#include <utility>

namespace kf {
namespace {
constexpr std::size_t texture_word_count = std::size_t(vram_width) * vram_height;
constexpr int texture_page_extent = 256;
constexpr int color5_bits = 5;
constexpr u32 color5_max = 31;
constexpr int color5_color8_shift = 3;
constexpr int color5_replication_shift = 2;
constexpr u16 texture_semitransparent_bit = 0x8000;
constexpr u8 texture_alpha_semitransparent_marker = 128;
constexpr u8 texture_alpha_opaque_marker = 255;

bool same_source(TextureSource a, TextureSource b) {
    return a.x == b.x && a.y == b.y && a.palette_x == b.palette_x &&
           a.palette_y == b.palette_y && a.format == b.format;
}
}

TextureSource texture_source_from_selectors(u16 tpage, u16 clut) {
    TextureSource source{};
    source.x = static_cast<u16>((tpage & 0x0f) * 64);
    source.y = static_cast<u16>((tpage & 0x10) * 16);
    const unsigned format = (tpage >> 7) & 3;
    source.format = format == 0 ? TextureFormat::Indexed4 : format == 1 ? TextureFormat::Indexed8 : TextureFormat::Direct16;
    source.palette_x = static_cast<u16>((clut & 0x3f) * 16);
    source.palette_y = static_cast<u16>((clut >> 6) & 0x1ff);
    return source;
}

bool texture_store_write(TextureStore *store, int x, int y, int width, int height, const u16 *words) try {
    if (store->words.empty())
        store->words.resize(texture_word_count);
    for (int row = 0; row < height; ++row)
        for (int column = 0; column < width; ++column)
            store->words[std::size_t((y + row) & (vram_height - 1)) * vram_width + ((x + column) & (vram_width - 1))] =
                words[std::size_t(row) * width + column];
    // Only entries whose page or palette intersect the rectangle need decoding again.
    const auto overlaps = [&](int left, int top, int span_x, int span_y) {
        return left < x + width && x < left + span_x && top < y + height && y < top + span_y;
    };
    for (auto &entry : store->entries) {
        const auto &source = entry.source;
        const int page_words = texture_page_extent >> (2 - static_cast<int>(source.format));
        const int palette_words = source.format == TextureFormat::Indexed4 ? 16 : 256;
        if (overlaps(source.x, source.y, page_words, texture_page_extent) ||
            (source.format != TextureFormat::Direct16 && overlaps(source.palette_x, source.palette_y, palette_words, 1)))
            entry.dirty = true;
    }
    return true;
} catch (const std::bad_alloc &) {
    return false;
}

bool texture_decode(Image *image, TextureSource source, const u16 *words, std::size_t count) try {
    const auto mode = static_cast<unsigned>(source.format);
    if (!words || count < texture_word_count || source.format > TextureFormat::Direct16)
        return false;
    Image decoded{texture_page_extent, texture_page_extent, {}};
    decoded.rgba.resize(texture_page_extent * texture_page_extent * 4);
    const unsigned pixels_per_word = 4 >> mode;
    for (unsigned y = 0; y < unsigned(texture_page_extent); ++y) {
        for (unsigned x = 0; x < unsigned(texture_page_extent); ++x) {
            auto value = words[((source.y + y) & (vram_height - 1)) * vram_width +
                               ((source.x + x / pixels_per_word) & (vram_width - 1))];
            if (source.format < TextureFormat::Direct16) {
                const unsigned bits = source.format == TextureFormat::Indexed4 ? 4 : 8;
                const unsigned index = (value >> ((x % pixels_per_word) * bits)) & ((1u << bits) - 1);
                value = words[(source.palette_y & (vram_height - 1)) * vram_width +
                              ((source.palette_x + index) & (vram_width - 1))];
            }
            auto *pixel = decoded.rgba.data() + (y * texture_page_extent + x) * 4;
            for (unsigned c = 0; c < 3; ++c) {
                const unsigned component = (value >> (c * color5_bits)) & color5_max;
                pixel[c] = static_cast<u8>((component << color5_color8_shift) | (component >> color5_replication_shift));
            }
            // Preserve the per-texel transparency category, not ordinary opacity.
            pixel[3] = value == 0 ? 0 : ((value & texture_semitransparent_bit) ? texture_alpha_semitransparent_marker
                                                                               : texture_alpha_opaque_marker);
        }
    }
    *image = std::move(decoded);
    return true;
} catch (const std::bad_alloc &) {
    return false;
}

u32 texture_store_resolve(TextureStore *store, TextureSource source) try {
    if (store->words.empty())
        store->words.resize(texture_word_count);
    if (source.format == TextureFormat::Direct16)
        source.palette_x = source.palette_y = 0;
    std::size_t index = 0;
    while (index < store->entries.size() && !same_source(store->entries[index].source, source))
        ++index;
    if (index == store->entries.size())
        store->entries.push_back({source, 0, true});
    auto *entry = &store->entries[index];
    if (entry->dirty) {
        Image decoded{};
        if (!texture_decode(&decoded, source, store->words.data(), store->words.size()))
            return 0;
        const auto texture = renderer_upload(&decoded);
        if (!texture)
            return 0;
        renderer_delete_texture(entry->texture);
        entry->texture = texture;
        entry->dirty = false;
    }
    return entry->texture;
} catch (const std::bad_alloc &) {
    return 0;
}

void texture_store_release(TextureStore *store) {
    for (auto &entry : store->entries)
        renderer_delete_texture(entry.texture);
    *store = {};
}
}
