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
  inherit psyqSdk gcc257Native gcc257Headers gcc260Native cc1psx257 cpppsx257 cc1psx260 cpppsx260
    aspsxNative asmpsxNative;
}
