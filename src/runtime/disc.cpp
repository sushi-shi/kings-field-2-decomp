#include "runtime.h"

#include <kf/platform/host.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// Read-only access to the user's original disc image: a single-track MODE2/2352
// BIN (optionally through its CUE sheet) or a 2048-byte ISO image.

namespace kf::psx {
namespace {
constexpr u32 sector_bytes = 2048;
constexpr u32 raw_sector_bytes = 2352;
constexpr u32 raw_mode2_data_offset = 24;
constexpr u32 volume_descriptor_lba = 16;
constexpr std::array<u8, 12> sync_pattern = {0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0};

struct Disc {
    std::FILE *file;
    u32 stride;
    u32 data_offset;
    u32 root_lba, root_size;
};
Disc disc;

u32 le32(const u8 *bytes) {
    return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (u32(bytes[3]) << 24);
}

std::string bin_from_cue(const char *path) {
    std::FILE *cue = std::fopen(path, "rb");
    if (!cue)
        return {};
    std::string text;
    std::array<char, 512> chunk{};
    std::size_t count;
    while ((count = std::fread(chunk.data(), 1, chunk.size(), cue)) > 0 && text.size() < 65536)
        text.append(chunk.data(), count);
    std::fclose(cue);
    const auto file = text.find("FILE \"");
    if (file == std::string::npos)
        return {};
    const auto end = text.find('"', file + 6);
    if (end == std::string::npos)
        return {};
    const auto name = text.substr(file + 6, end - file - 6);
    if (name.find('/') != std::string::npos || name.find('\\') != std::string::npos)
        return {};
    std::string directory = path;
    const auto slash = directory.find_last_of('/');
    directory = slash == std::string::npos ? std::string{} : directory.substr(0, slash + 1);
    return directory + name;
}

bool upper_equal(const std::string &a, const char *b, std::size_t length) {
    if (a.size() != length)
        return false;
    for (std::size_t i = 0; i < length; ++i)
        if (std::toupper(static_cast<unsigned char>(a[i])) != std::toupper(static_cast<unsigned char>(b[i])))
            return false;
    return true;
}
}

bool disc_open(const char *path) {
    std::string image = path;
    if (image.size() > 4 && (image.ends_with(".cue") || image.ends_with(".CUE")))
        image = bin_from_cue(path);
    if (image.empty()) {
        std::fprintf(stderr, "Cannot read the CUE sheet: %s\n", path);
        return false;
    }
    disc.file = std::fopen(image.c_str(), "rb");
    if (!disc.file) {
        std::fprintf(stderr, "Cannot open the disc image: %s\n", image.c_str());
        return false;
    }
    std::array<u8, 12> head{};
    if (std::fread(head.data(), 1, head.size(), disc.file) != head.size())
        return false;
    if (head == sync_pattern) {
        disc.stride = raw_sector_bytes;
        disc.data_offset = raw_mode2_data_offset;
    } else {
        disc.stride = sector_bytes;
        disc.data_offset = 0;
    }
    std::array<u8, sector_bytes> descriptor{};
    if (!disc_read_sector(volume_descriptor_lba, descriptor.data()) || descriptor[0] != 1 ||
        std::memcmp(descriptor.data() + 1, "CD001", 5) != 0) {
        std::fprintf(stderr, "The disc image has no ISO9660 volume.\n");
        return false;
    }
    if (std::memcmp(descriptor.data() + 40, "SLPS_00069", 10) != 0 &&
        std::memcmp(descriptor.data() + 40, "SLPS-00069", 10) != 0)
        std::fprintf(stderr, "Warning: volume identifier is not SLPS-00069.\n");
    disc.root_lba = le32(descriptor.data() + 156 + 2);
    disc.root_size = le32(descriptor.data() + 156 + 10);
    return true;
}

bool disc_read_sector(u32 lba, u8 *destination) {
    if (!disc.file)
        return false;
    if (std::fseek(disc.file, static_cast<long>(std::uint64_t(lba) * disc.stride + disc.data_offset), SEEK_SET) != 0)
        return false;
    return std::fread(destination, 1, sector_bytes, disc.file) == sector_bytes;
}

bool disc_find(const char *path, u32 *lba, u32 *size) {
    std::string name = path;
    if (name.rfind("cdrom:", 0) == 0 || name.rfind("CDROM:", 0) == 0)
        name = name.substr(6);
    std::replace(name.begin(), name.end(), '/', '\\');
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (start <= name.size()) {
        const auto next = name.find('\\', start);
        const auto part = name.substr(start, next == std::string::npos ? std::string::npos : next - start);
        if (!part.empty())
            parts.push_back(part);
        if (next == std::string::npos)
            break;
        start = next + 1;
    }
    if (parts.empty())
        return false;
    if (parts.back().find(';') == std::string::npos)
        parts.back() += ";1";
    u32 directory_lba = disc.root_lba, directory_size = disc.root_size;
    std::array<u8, sector_bytes> sector{};
    for (std::size_t depth = 0; depth < parts.size(); ++depth) {
        const bool last = depth + 1 == parts.size();
        bool found = false;
        for (u32 offset = 0; offset < directory_size && !found; offset += sector_bytes) {
            if (!disc_read_sector(directory_lba + offset / sector_bytes, sector.data()))
                return false;
            for (u32 position = 0; position < sector_bytes;) {
                const u8 length = sector[position];
                if (length == 0 || position + length > sector_bytes)
                    break;
                const u8 name_length = sector[position + 32];
                const char *record_name = reinterpret_cast<const char *>(sector.data() + position + 33);
                const bool is_directory = (sector[position + 25] & 2) != 0;
                if (upper_equal(parts[depth], record_name, name_length) && is_directory != last) {
                    directory_lba = le32(sector.data() + position + 2);
                    directory_size = le32(sector.data() + position + 10);
                    found = true;
                    break;
                }
                position += length;
            }
        }
        if (!found)
            return false;
    }
    *lba = directory_lba;
    *size = directory_size;
    return true;
}
}
