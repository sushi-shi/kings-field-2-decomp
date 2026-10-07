#include <kf/renderer/renderer.h>

#include <GLES3/gl3.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

// Adapted from the King's Field port renderer. The color target here is the
// whole frame buffer, so drawing areas, image transfers and the display area
// share one coordinate space. GL rows equal frame-buffer rows; presentation flips.

namespace kf {
namespace {
constexpr int display_aspect_width = 4;
constexpr int display_aspect_height = 3;
constexpr GLint shader_texture_solid = 0;
constexpr GLint shader_texture_modulated = 1;
constexpr GLint shader_texture_raw = 2;
constexpr float maximum_triangle_width = 1023;
constexpr float maximum_triangle_height = 511;

GLuint shader(GLenum type, const char *source, char *error, std::size_t size) {
    const auto object = glCreateShader(type);
    glShaderSource(object, 1, &source, nullptr);
    glCompileShader(object);
    GLint good = 0;
    glGetShaderiv(object, GL_COMPILE_STATUS, &good);
    if (!good) {
        glGetShaderInfoLog(object, static_cast<GLsizei>(size), nullptr, error);
        glDeleteShader(object);
        return 0;
    }
    return object;
}

GLuint make_target_texture() {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, vram_width, vram_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

u8 expand5(unsigned value) {
    return static_cast<u8>((value << 3) | (value >> 2));
}
}

bool renderer_init(Renderer *renderer, char *error, std::size_t size) {
    const auto vertex = shader(GL_VERTEX_SHADER, R"(#version 300 es
layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec4 color;
uniform int texture_mode;
out vec2 uv; out vec4 tint;
const int texture_solid=0;
const float rgb8_max=255.0;
const float modulation_unity=128.0;
void main() {
    gl_Position=vec4(position.x/512.0-1.0,position.y/256.0-1.0,0.0,1.0);
    uv=texcoord;
    tint=vec4(floor(color.rgb*(texture_mode==texture_solid ? rgb8_max : modulation_unity)+0.5),color.a);
}
)", error, size);
    if (!vertex)
        return false;
    const auto fragment = shader(GL_FRAGMENT_SHADER, R"(#version 300 es
precision highp float;
precision highp int;
uniform sampler2D image;
uniform sampler2D backdrop;
uniform int texture_mode;
uniform int dither_enabled;
uniform int blend_mode;
in vec2 uv; in vec4 tint;
out vec4 pixel;
const int texture_solid=0, texture_modulated=1, texture_raw=2;
const int blend_opaque=0, blend_average=1, blend_add=2, blend_subtract=3, blend_add_quarter=4;
const float rgb8_max=255.0;
const int rgb5_max=31, rgb_expansion_shift=3, rgb_replication_shift=2;
const int modulation_shift=4;
const int dither_period=4, dither_mask=dither_period-1;
const float stp_alpha_threshold=0.75;
const int dither_offsets[dither_period*dither_period]=int[dither_period*dither_period](
    -4,0,-3,1, 2,-2,3,-1, -3,1,-4,0, 3,-1,2,-2);
void main() {
    vec4 sample_color=texture(image,uv);
    if(sample_color.a==0.0) discard;
    bool stp=texture_mode!=texture_solid && sample_color.a<stp_alpha_threshold;
    ivec3 shade=ivec3(clamp(floor(tint.rgb),0.0,rgb8_max));
    ivec3 texel=ivec3(floor(sample_color.rgb*float(rgb5_max)+0.5));
    ivec3 foreground=shade;
    if(texture_mode==texture_modulated) foreground=(texel*shade)>>modulation_shift;
    if(texture_mode==texture_raw) foreground=texel<<rgb_expansion_shift;
    ivec2 destination=ivec2(gl_FragCoord.xy);
    int offset=0;
    if(dither_enabled!=0)
        offset=dither_offsets[(destination.y&dither_mask)*dither_period+(destination.x&dither_mask)];
    foreground=clamp((foreground+ivec3(offset))>>rgb_expansion_shift,ivec3(0),ivec3(rgb5_max));
    if(blend_mode!=blend_opaque && (texture_mode==texture_solid || stp)) {
        ivec3 background=ivec3(floor(texelFetch(backdrop,destination,0).rgb*float(rgb5_max)+0.5));
        if(blend_mode==blend_average) foreground=(background+foreground)>>1;
        if(blend_mode==blend_add) foreground=background+foreground;
        if(blend_mode==blend_subtract) foreground=background-foreground;
        if(blend_mode==blend_add_quarter) foreground=background+(foreground>>2);
        foreground=clamp(foreground,ivec3(0),ivec3(rgb5_max));
    }
    ivec3 expanded=(foreground<<rgb_expansion_shift)|(foreground>>rgb_replication_shift);
    pixel=vec4(vec3(expanded)/rgb8_max,stp ? 1.0 : 0.0);
}
)", error, size);
    if (!fragment) {
        glDeleteShader(vertex);
        return false;
    }
    renderer->program = glCreateProgram();
    glAttachShader(renderer->program, vertex);
    glAttachShader(renderer->program, fragment);
    glLinkProgram(renderer->program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    GLint good = 0;
    glGetProgramiv(renderer->program, GL_LINK_STATUS, &good);
    if (!good) {
        glGetProgramInfoLog(renderer->program, static_cast<GLsizei>(size), nullptr, error);
        renderer_release(renderer);
        return false;
    }
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);
    glGenBuffers(1, &renderer->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, u)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, r)));
    renderer->color_texture = make_target_texture();
    glGenFramebuffers(1, &renderer->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->color_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::snprintf(error, size, "Cannot create the frame-buffer render target.");
        renderer_release(renderer);
        return false;
    }
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    renderer->blend_texture = make_target_texture();
    const Image solid{1, 1, {255, 255, 255, 255}};
    renderer->white_texture = renderer_upload(&solid);
    if (!renderer->white_texture) {
        std::snprintf(error, size, "Cannot create solid-color material.");
        renderer_release(renderer);
        return false;
    }
    glUseProgram(renderer->program);
    glUniform1i(glGetUniformLocation(renderer->program, "image"), 0);
    glUniform1i(glGetUniformLocation(renderer->program, "backdrop"), 1);
    renderer->display = {0, 0, 320, 240};
    return glGetError() == GL_NO_ERROR;
}

void renderer_release(Renderer *renderer) {
    texture_store_release(&renderer->textures);
    renderer_delete_texture(renderer->white_texture);
    glDeleteProgram(renderer->program);
    glDeleteBuffers(1, &renderer->buffer);
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteTextures(1, &renderer->color_texture);
    glDeleteTextures(1, &renderer->blend_texture);
    glDeleteFramebuffers(1, &renderer->framebuffer);
    *renderer = {};
}

TextureId renderer_upload(const Image *image) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(image->width), static_cast<GLsizei>(image->height),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image->rgba.data());
    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &texture);
        return 0;
    }
    return texture;
}

void renderer_delete_texture(TextureId texture) {
    glDeleteTextures(1, &texture);
}

bool renderer_draw_faces(Renderer *renderer, const DrawFace *faces, std::size_t count, const DrawArea &area) {
    const int left = std::clamp(area.x, 0, vram_width);
    const int top = std::clamp(area.y, 0, vram_height);
    const int right = std::clamp(area.x + area.width, left, vram_width);
    const int bottom = std::clamp(area.y + area.height, top, vram_height);
    if (right <= left || bottom <= top || count == 0)
        return true;
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer);
    glViewport(0, 0, vram_width, vram_height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(left, top, right - left, bottom - top);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(renderer->program);
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->buffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderer->blend_texture);
    const auto texture_mode = glGetUniformLocation(renderer->program, "texture_mode");
    const auto dither_enabled = glGetUniformLocation(renderer->program, "dither_enabled");
    const auto blend_mode = glGetUniformLocation(renderer->program, "blend_mode");
    bool good = true;
    for (std::size_t f = 0; f < count; ++f) {
        const auto &face = faces[f];
        const bool textured = face.material.kind == SurfaceKind::Texture;
        const auto texture = textured ? texture_store_resolve(&renderer->textures, face.material.source)
                                      : renderer->white_texture;
        if (!texture) {
            good = false;
            continue;
        }
        const bool dither = area.dither && (textured ? face.material.color_mode == TextureColorMode::Modulated
                                                     : face.shading == FaceShading::Gouraud);
        constexpr std::array<std::array<int, 3>, 2> halves = {{{0, 1, 2}, {1, 3, 2}}};
        const int triangle_count = face.shape == FaceShape::Quad ? 2 : 1;
        for (int t = 0; t < triangle_count; ++t) {
            const std::array<Vertex, 3> vertices = {face.vertices[halves[t][0]], face.vertices[halves[t][1]],
                                                    face.vertices[halves[t][2]]};
            const float min_x = std::min({vertices[0].x, vertices[1].x, vertices[2].x});
            const float max_x = std::max({vertices[0].x, vertices[1].x, vertices[2].x});
            const float min_y = std::min({vertices[0].y, vertices[1].y, vertices[2].y});
            const float max_y = std::max({vertices[0].y, vertices[1].y, vertices[2].y});
            // The original rasterizer rejects oversized triangles before clipping.
            if (max_x - min_x > maximum_triangle_width || max_y - min_y > maximum_triangle_height)
                continue;
            if (face.material.blend != BlendMode::opaque) {
                const auto x0 = std::clamp(static_cast<int>(std::floor(min_x)), left, right);
                const auto x1 = std::clamp(static_cast<int>(std::ceil(max_x)), left, right);
                const auto y0 = std::clamp(static_cast<int>(std::floor(min_y)), top, bottom);
                const auto y1 = std::clamp(static_cast<int>(std::ceil(max_y)), top, bottom);
                if (x1 <= x0 || y1 <= y0)
                    continue;
                // Reading the attached target in a shader is undefined; snapshot the bounds.
                glActiveTexture(GL_TEXTURE1);
                glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x0, y0, x0, y0, x1 - x0, y1 - y0);
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices.data(), GL_STREAM_DRAW);
            glUniform1i(texture_mode, !textured ? shader_texture_solid
                                     : face.material.color_mode == TextureColorMode::Modulated ? shader_texture_modulated
                                                                                              : shader_texture_raw);
            glUniform1i(dither_enabled, dither ? 1 : 0);
            glUniform1i(blend_mode, static_cast<GLint>(face.material.blend));
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return good;
}

void renderer_fill(Renderer *renderer, int x, int y, int width, int height, u8 red, u8 green, u8 blue) {
    x = std::clamp(x, 0, vram_width);
    y = std::clamp(y, 0, vram_height);
    width = std::clamp(width, 0, vram_width - x);
    height = std::clamp(height, 0, vram_height - y);
    if (!width || !height)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer);
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
    glClearColor(expand5(red >> 3) / 255.0f, expand5(green >> 3) / 255.0f, expand5(blue >> 3) / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const u16 word = static_cast<u16>((red >> 3) | ((green >> 3) << 5) | ((blue >> 3) << 10));
    std::vector<u16> words(std::size_t(width) * height, word);
    texture_store_write(&renderer->textures, x, y, width, height, words.data());
}

void renderer_write_vram(Renderer *renderer, int x, int y, int width, int height, const u16 *words) {
    if (width <= 0 || height <= 0)
        return;
    texture_store_write(&renderer->textures, x, y, width, height, words);
    std::vector<u8> rgba(std::size_t(width) * height * 4);
    for (std::size_t i = 0; i < std::size_t(width) * height; ++i) {
        const auto word = words[i];
        rgba[i * 4 + 0] = expand5(word & 31);
        rgba[i * 4 + 1] = expand5((word >> 5) & 31);
        rgba[i * 4 + 2] = expand5((word >> 10) & 31);
        rgba[i * 4 + 3] = (word & 0x8000) ? 255 : 0;
    }
    glBindTexture(GL_TEXTURE_2D, renderer->color_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Transfers wrap at the frame-buffer edges; split into in-bounds pieces.
    for (int row = 0; row < height; ++row) {
        const int ty = (y + row) & (vram_height - 1);
        int column = 0;
        while (column < width) {
            const int tx = (x + column) & (vram_width - 1);
            const int run = std::min(width - column, vram_width - tx);
            glTexSubImage2D(GL_TEXTURE_2D, 0, tx, ty, run, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                            rgba.data() + (std::size_t(row) * width + column) * 4);
            column += run;
        }
    }
}

void renderer_read_vram(Renderer *renderer, int x, int y, int width, int height, u16 *words) {
    if (width <= 0 || height <= 0)
        return;
    std::vector<u8> rgba(std::size_t(width) * height * 4);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer->framebuffer);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    for (int row = 0; row < height; ++row) {
        const int ty = (y + row) & (vram_height - 1);
        int column = 0;
        while (column < width) {
            const int tx = (x + column) & (vram_width - 1);
            const int run = std::min(width - column, vram_width - tx);
            glReadPixels(tx, ty, run, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data() + (std::size_t(row) * width + column) * 4);
            column += run;
        }
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    for (std::size_t i = 0; i < std::size_t(width) * height; ++i)
        words[i] = static_cast<u16>((rgba[i * 4] >> 3) | ((rgba[i * 4 + 1] >> 3) << 5) | ((rgba[i * 4 + 2] >> 3) << 10) |
                                    (rgba[i * 4 + 3] >= 128 ? 0x8000 : 0));
}

void renderer_set_display(Renderer *renderer, DisplayArea display) {
    renderer->display = display;
}

void renderer_present(const Renderer *renderer, int width, int height) {
    if (width <= 0 || height <= 0)
        return;
    const auto &display = renderer->display;
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    int target_width = width, target_height = width * display_aspect_height / display_aspect_width;
    if (target_height > height) {
        target_height = height;
        target_width = height * display_aspect_width / display_aspect_height;
    }
    const int x = (width - target_width) / 2, y = (height - target_height) / 2;
    const int source_x = std::clamp(display.x, 0, vram_width);
    const int source_y = std::clamp(display.y, 0, vram_height);
    const int source_right = std::clamp(display.x + display.width, source_x, vram_width);
    const int source_bottom = std::clamp(display.y + display.height, source_y, vram_height);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer->framebuffer);
    // Frame-buffer row zero is the top of the display; window rows grow upward.
    glBlitFramebuffer(source_x, source_y, source_right, source_bottom, x, y + target_height, x + target_width, y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace kf
