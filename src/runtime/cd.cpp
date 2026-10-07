#include "runtime.h"

#include <kf/psx/sdk.h>

#include <array>
#include <cstring>
#include <vector>

// CD-ROM library calls served synchronously from the disc image. Reads finish
// before the call returns; completion callbacks are not interrupts here.

namespace kf::psx {
namespace {
constexpr u32 sector_bytes = 2048;
constexpr u32 pregap_sectors = 150;
constexpr u32 stream_header_bytes = 32;
constexpr u32 stream_payload_bytes = sector_bytes - stream_header_bytes;
constexpr u16 stream_magic = 0x0160;

struct CdState {
    u32 location;
    u32 mode;
    bool reading;
    bool streaming;
    std::array<u8, sector_bytes> sector;
    u32 sector_offset;
    void (*ready_callback)();
    void (*data_callback)();
    std::vector<u8> frame;
    std::array<u8, stream_header_bytes> frame_header;
    bool frame_locked;
};
CdState cd;

u32 from_bcd(u8 value) { return (value >> 4) * 10 + (value & 15); }
u8 to_bcd(u32 value) { return static_cast<u8>((value / 10) << 4 | (value % 10)); }

u32 location_to_lba(const u_char *location) {
    const u32 absolute = (from_bcd(location[0]) * 60 + from_bcd(location[1])) * 75 + from_bcd(location[2]);
    return absolute >= pregap_sectors ? absolute - pregap_sectors : 0;
}

void lba_to_location(u32 lba, CdlLOC *location) {
    const u32 absolute = lba + pregap_sectors;
    location->minute = to_bcd(absolute / 75 / 60);
    location->second = to_bcd(absolute / 75 % 60);
    location->sector = to_bcd(absolute % 75);
    location->track = 0;
}

u16 le16(const u8 *bytes) { return static_cast<u16>(bytes[0] | (bytes[1] << 8)); }
}

bool cd_service_stream() {
    // Interrupt-driven reading delivers one data-ready event per sector.
    if (!cd.reading || !interrupt_enabled(event_class_cdrom, event_spec_cd_data_ready))
        return false;
    if (!disc_read_sector(cd.location, cd.sector.data())) {
        cd.reading = false;
        return false;
    }
    ++cd.location;
    cd.sector_offset = 0;
    interrupt_raise(event_class_cdrom, event_spec_cd_data_ready);
    return true;
}
}

using namespace kf::psx;

extern "C" {
void CdInit(void) {
    cd.location = 0;
    cd.reading = cd.streaming = false;
}

CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    kf::u32 lba = 0, size = 0;
    if (!disc_find(name, &lba, &size))
        return nullptr;
    lba_to_location(lba, &fp->pos);
    fp->size = size;
    std::memset(fp->name, 0, sizeof fp->name);
    const char *base = std::strrchr(name, '\\');
    std::strncpy(fp->name, base ? base + 1 : name, sizeof fp->name - 1);
    return fp;
}

int CdControl(u_char com, u_char *param, u_char *result) {
    if (result)
        result[0] = cd.reading ? 0x22 : 0x02;
    switch (com) {
    case CdlSetloc:
        if (param)
            cd.location = location_to_lba(param);
        break;
    case CdlSeekL:
    case CdlSeekP:
        if (param)
            cd.location = location_to_lba(param);
        cd.reading = false;
        interrupt_raise(event_class_cdrom, event_spec_cd_complete);
        break;
    case CdlReadN:
    case CdlReadS:
        if (param)
            cd.location = location_to_lba(param);
        cd.reading = true;
        cd.sector_offset = sector_bytes;
        break;
    case CdlPause:
    case CdlStop:
    case CdlStandby:
        cd.reading = cd.streaming = false;
        interrupt_raise(event_class_cdrom, event_spec_cd_complete);
        break;
    case CdlSetmode:
        if (param)
            cd.mode = param[0];
        break;
    default:
        break;
    }
    return 1;
}

int CdControlB(u_char com, u_char *param, u_char *result) {
    return CdControl(com, param, result);
}

int CdReady(int, u_char *result) {
    if (!cd.reading)
        return CdlDiskError;
    if (!disc_read_sector(cd.location, cd.sector.data())) {
        cd.reading = false;
        return CdlDiskError;
    }
    ++cd.location;
    cd.sector_offset = 0;
    if (result)
        result[0] = 0x22;
    return CdlDataReady;
}

int CdGetSector(void *madr, int size) {
    const kf::u32 bytes = static_cast<kf::u32>(size) * 4;
    if (cd.sector_offset + bytes > sector_bytes)
        return 0;
    std::memcpy(madr, cd.sector.data() + cd.sector_offset, bytes);
    cd.sector_offset += bytes;
    return 1;
}

int CdRead(int sectors, u_long *buf, int mode) {
    cd.mode = static_cast<kf::u32>(mode);
    auto *destination = reinterpret_cast<kf::u8 *>(buf);
    for (int i = 0; i < sectors; ++i) {
        if (!disc_read_sector(cd.location, destination + std::size_t(i) * sector_bytes))
            return 0;
        ++cd.location;
    }
    // The library pauses after the transfer; that command completes as an event.
    interrupt_raise(event_class_cdrom, event_spec_cd_complete);
    return 1;
}

int CdReadSync(int, u_char *result) {
    if (result)
        result[0] = 0x02;
    return 0;
}

int CdRead2(long mode) {
    cd.mode = static_cast<kf::u32>(mode);
    cd.reading = true;
    cd.streaming = true;
    return 1;
}

u_long CdReadyCallback(void (*func)()) {
    const auto previous = cd.ready_callback;
    cd.ready_callback = func;
    return reinterpret_cast<u_long>(previous);
}

int CdDataCallback(void (*func)()) {
    const auto previous = cd.data_callback;
    cd.data_callback = func;
    return static_cast<int>(reinterpret_cast<u_long>(previous));
}

void StSetRing(u_long *, u_long) {
    cd.frame_locked = false;
}

void StSetStream(u_long, u_long, u_long, void (*)(), void (*)()) {
    cd.frame_locked = false;
}

// Assembles the next movie frame from consecutive stream sectors. The frame is
// held in runtime storage until StFreeRing releases it.
u_long StGetNext(u_long **addr, u_long **header) {
    if (!cd.streaming || cd.frame_locked)
        return 1;
    std::array<kf::u8, sector_bytes> sector{};
    for (int guard = 0; guard < 64; ++guard) {
        if (!disc_read_sector(cd.location, sector.data())) {
            cd.streaming = false;
            return 1;
        }
        ++cd.location;
        if (le16(sector.data()) != stream_magic)
            continue;
        const kf::u16 index = le16(sector.data() + 4);
        const kf::u16 count = le16(sector.data() + 6);
        if (index != 0 || count == 0)
            continue;
        std::memcpy(cd.frame_header.data(), sector.data(), stream_header_bytes);
        cd.frame.assign(std::size_t(count) * stream_payload_bytes, 0);
        std::memcpy(cd.frame.data(), sector.data() + stream_header_bytes, stream_payload_bytes);
        for (kf::u16 i = 1; i < count; ++i) {
            if (!disc_read_sector(cd.location, sector.data()))
                break;
            ++cd.location;
            std::memcpy(cd.frame.data() + std::size_t(i) * stream_payload_bytes, sector.data() + stream_header_bytes,
                        stream_payload_bytes);
        }
        *addr = reinterpret_cast<u_long *>(cd.frame.data());
        *header = reinterpret_cast<u_long *>(cd.frame_header.data());
        cd.frame_locked = true;
        return 0;
    }
    return 1;
}

u_long StFreeRing(u_long *) {
    cd.frame_locked = false;
    return 0;
}
}
