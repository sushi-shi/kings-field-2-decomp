{ pkgs, sdkBuilder }:
let
      # Psy-Q PS-X Development System Release 3.0 kit; files dated 1995-04-08
      # despite the Archive.org item title. Its libraries carry the exact Sony
      # RCS revisions embedded in the retail JP/US executables.
      psyqKit = pkgs.fetchurl {
        name = "psyq-3.0-psx.zip";
        url = "https://archive.org/download/psyq_psx_toolchain_april_08_1994/psx.zip";
        hash = "sha256-QWJBY3zbAnO2vZNrCrVifIunIl7iuYBmzztQ+wnleJk=";
      };

      # Native Decompals rebuild used only as a practical code-generation probe.
      # It corresponds to GCC 2.6.0's PSX target, but is not evidence that this
      # rebuilt host binary (or either staged historical 2.6.0 binary) built KF.
      gcc260NativeArchive = pkgs.fetchurl {
        name = "decompals-old-gcc-0.17-gcc-2.6.0-psx.tar.gz";
        url = "https://github.com/decompals/old-gcc/releases/download/0.17/gcc-2.6.0-psx.tar.gz";
        hash = "sha256-NY2slJ8PVmr5xq+Tu6DUrATVbKnfgMWL1m2Bu7UN7Os=";
      };
      gcc260Native = pkgs.runCommand "decompals-gcc-2.6.0-psx-0.17" {
        nativeBuildInputs = [ pkgs.gnutar pkgs.gzip ];
      } ''
        mkdir -p "$out/bin"
        tar -xzf ${gcc260NativeArchive} -C "$out/bin"
        chmod +x "$out/bin"/*
      '';

      # Second native probe. GCC 2.5.7 is the closest available rebuild to the
      # 1994 Psy-Q compiler family; its text epilogue and load hoisting match
      # retail forms that 2.6.0 cannot emit, but it is still a probe.
      gcc257NativeArchive = pkgs.fetchurl {
        name = "decompals-old-gcc-0.17-gcc-2.5.7-psx.tar.gz";
        url = "https://github.com/decompals/old-gcc/releases/download/0.17/gcc-2.5.7-psx.tar.gz";
        hash = "sha256-DbH7QDx2blGwlDVLmhNqbB8BRXqkHMyiisRXt8tn2HE=";
      };
      gcc257Native = pkgs.runCommand "decompals-gcc-2.5.7-psx-0.17" {
        nativeBuildInputs = [ pkgs.gnutar pkgs.gzip ];
      } ''
        mkdir -p "$out/bin"
        tar -xzf ${gcc257NativeArchive} -C "$out/bin"
        chmod +x "$out/bin"/*
      '';

      gcc257SourceArchive = pkgs.fetchurl {
        url = "https://www.nic.funet.fi/index/gnu/funet/historical-funet-gnu-area-from-early-1990s/gcc-2.5.7.tar.gz";
        hash = "sha256-Y0W+QiNeXsTESNZYflUoPaPuV3YdtdINegpI1jmH/I4=";
      };
      gcc257Headers = pkgs.runCommand "gcc-2.5.7-mips-headers" {
        nativeBuildInputs = [ pkgs.gnutar pkgs.gzip ];
      } ''
        mkdir -p "$out/include"
        tar -xzf ${gcc257SourceArchive} --strip-components=1 -C "$out/include" \
          gcc-2.5.7/gstdarg.h gcc-2.5.7/va-mips.h
        mv "$out/include/gstdarg.h" "$out/include/stdarg.h"
      '';

      # The one active historical SDK is the complete pinned Psy-Q 3.0 kit
      # tree. Analysis programs below do not enter this derivation.
      psyqSdk = pkgs.runCommand "kings-field-2-psyq-3.0-sdk" {
        nativeBuildInputs = with pkgs; [
          binutils
          coreutils
          file
          p7zip
          python3
          unar
        ];
      } ''
        export PSYQ_KIT_ZIP="${psyqKit}"
        python3 ${sdkBuilder} \
          --work-dir "$TMPDIR/toolchain-work" \
          --stage-dir "$out"
      '';

      # This Release 2.5 standalone allocator is absent from the active 3.0
      # kit. Its 0x60c text and all 1172 fixed bytes match the linked allocator
      # in OPEN and END; the original distribution version remains unproved.
      psyq25Floppies = pkgs.fetchurl {
        name = "psyq-release-2.5-floppies.rar";
        url = "https://archive.org/download/ps1_sdks/Floppies.rar";
        hash = "sha256-SaLzzryjqEIclPHeQ7nSDnN1CwQrCDHdKxcy0o+Ud4M=";
      };
      psyqMallocObj = pkgs.runCommand "kings-field-2-psyq-release-2.5-malloc" {
        nativeBuildInputs = with pkgs; [ coreutils findutils unar ];
      } ''
        mkdir -p "$TMPDIR/floppies" "$out"
        unar -quiet -force-overwrite -output-directory "$TMPDIR/floppies" ${psyq25Floppies}
        malloc_obj="$(find "$TMPDIR/floppies" -type f -path '*/isa board/PSXLIB/LIB/MALLOC.OBJ' -print -quit)"
        test -n "$malloc_obj"
        test "$(sha256sum "$malloc_obj" | cut -d' ' -f1)" = \
          "628e405fd0e3acfff2ce9d4a15d481f0aa36398c14e9eae0a82b7ff0a86a74c9"
        cp "$malloc_obj" "$out/MALLOC.OBJ"
      '';

      # Sony's Programmer Tool Runtime Library 3.0 CD (DTL-S2180, March 1995).
      # Its PSXGRAPH CPE2X 1.3 is a later build than the kit's: it zeroes the
      # EXEC save area and reserved words and sets s_addr to 801ffff0, as the
      # retail PSX, OPEN and END headers record. Only this file is extracted.
      psyqRuntime30Cd = pkgs.fetchurl {
        name = "psx-runtime-library-3.0-dtl-s2180.zip";
        url = "https://archive.org/download/ps1_sdks/Programmer%20Tool%20-%20Runtime%20Library%20Version%203.0%20%28Japan%29%20%28En%2CJa%29_DTL-S2180_redump.zip";
        hash = "sha256-BxeoIBl9M35TaWy6uuShJS9eIDOWhtMMWSm+N/r07zc=";
      };
      psyqRuntime30Cpe2x = pkgs.runCommand "kings-field-2-runtime-3.0-cpe2x" {
        nativeBuildInputs = with pkgs; [ coreutils python3 ];
      } ''
        mkdir -p "$out"
        # Read PSXGRAPH/BIN/CPE2X.EXE from the MODE1/2352 track's ISO 9660 tree.
        python3 - ${psyqRuntime30Cd} "$out/CPE2X.EXE" <<'PY'
        import struct, sys, zipfile
        with zipfile.ZipFile(sys.argv[1]) as archive:
            name = next(n for n in archive.namelist() if n.endswith('.bin'))
            raw = archive.read(name)
        assert len(raw) % 2352 == 0
        def sector(lba):
            return raw[lba * 2352 + 16:lba * 2352 + 2064]
        def read(lba, size):
            return bytes().join(sector(lba + i) for i in range((size + 2047) // 2048))[:size]
        def entries(lba, size):
            data = read(lba, size)
            position = 0
            while position < len(data):
                length = data[position]
                if not length:
                    position = (position // 2048 + 1) * 2048
                    continue
                record = data[position:position + length]
                extent, extent_size = struct.unpack_from('<I', record, 2)[0], struct.unpack_from('<I', record, 10)[0]
                identifier = record[33:33 + record[32]].decode('ascii').split(';')[0]
                yield identifier, extent, extent_size
                position += length
        descriptor = sector(16)
        assert descriptor[1:6] == b'CD001'
        root = descriptor[156:156 + 34]
        lba, size = struct.unpack_from('<I', root, 2)[0], struct.unpack_from('<I', root, 10)[0]
        for part in ('PSXGRAPH', 'BIN', 'CPE2X.EXE'):
            lba, size = next((e, n) for i, e, n in entries(lba, size) if i == part)
        open(sys.argv[2], 'wb').write(read(lba, size))
        PY
        test "$(sha256sum "$out/CPE2X.EXE" | cut -d' ' -f1)" = \
          "641d95ebe8131c3503407518cb6110ed311cb5f87943d866296660ab98938af2"
      '';

      # Inherited from the King's Field (SLPS-00017) setup, where the Release
      # 2.5 ASPSX was software-key protected. This hash-pinned ASPSX 1.07
      # assembles all compiler output; its distinct provenance remains explicit
      # in every build report. The 3.0 kit's own ASPSX 2.08 is not yet wired in.
      aspsxArchive = pkgs.fetchurl {
        name = "aspsx-binaries.tar.gz";
        url = "https://github.com/mkst/maspsx/releases/download/aspsx/aspsx-binaries.tar.gz";
        hash = "sha256-fHU4wq+SMzjdxaevXFfSgLGTBmDpj5xDDm6iq3XlLno=";
      };
      aspsxNative = pkgs.runCommand "kings-field-aspsx-1.07" {
        nativeBuildInputs = [ pkgs.gnutar pkgs.gzip ];
      } ''
        mkdir -p "$out"
        tar -xzf ${aspsxArchive} -C "$out" ./1.07/ASPSX.EXE
      '';

      # The C assembler lacks named sections, while the kit's macro assembler
      # is not yet validated under DOSBox. Use this preserved native ASMPSX
      # solely for zero-byte linker boundary declarations, not game bodies.
      asmpsxNative = pkgs.fetchurl {
        name = "kings-field-asmpsx-2.34.exe";
        url = "https://raw.githubusercontent.com/HighwayFrogs/frogger-psx/fa2d5185b19ae89aaceeb47b2828369e06566edf/sdk/bin/SDK4.0/DOS/ASMPSX.EXE";
        sha256 = "c27e07db59f29282c837e06204a3a5efe69183dbfd2570c8b0d46f1cb5f760bb";
      };

      cc1psx260 = pkgs.writeShellApplication {
        name = "cc1psx-260";
        text = ''exec ${gcc260Native}/bin/cc1 "$@"'';
      };

      cpppsx260 = pkgs.writeShellApplication {
        name = "cpppsx-260";
        text = ''exec ${gcc260Native}/bin/cpp "$@"'';
      };

      cc1psx257 = pkgs.writeShellApplication {
        name = "cc1psx-257";
        text = ''exec ${gcc257Native}/bin/cc1 "$@"'';
      };

      cpppsx257 = pkgs.writeShellApplication {
        name = "cpppsx-257";
        text = ''exec ${gcc257Native}/bin/cpp "$@"'';
      };

in {
  inherit psyqSdk psyqMallocObj psyqRuntime30Cpe2x gcc257Native gcc257Headers gcc260Native cc1psx257 cpppsx257 cc1psx260 cpppsx260
    aspsxNative asmpsxNative;
}
