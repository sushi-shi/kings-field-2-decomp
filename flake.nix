{
  description = "King's Field II portable Linux and WebAssembly application";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/64c08a7ca051951c8eae34e3e3cb1e202fe36786";
  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      # The original sources assume 32-bit pointers; build an i686 executable,
      # which runs on x86_64 Linux with 32-bit graphics drivers.
      pkgs32 = pkgs.pkgsi686Linux;
      nativeTools = with pkgs; [ cmake ninja pkg-config python3 ];
      libraries32 = with pkgs32; [ sdl3 libGL libglvnd ];
      sources = pkgs.lib.cleanSourceWith {
        src = ./.;
        filter = path: type:
          let
            relative = pkgs.lib.removePrefix "${toString ./.}/" (toString path);
            sourceDirectories = [ "src" "include" "cmake" "web" ];
          in pkgs.lib.cleanSourceFilter path type
            && !(builtins.elem (baseNameOf path) [ "build" "result" ])
            && (builtins.elem relative [ "CMakeLists.txt" "build.json" ]
              || builtins.any (directory:
                relative == directory || pkgs.lib.hasPrefix "${directory}/" relative
              ) sourceDirectories);
      };
      unwrapped = pkgs32.clangStdenv.mkDerivation {
        pname = "kings-field-2";
        version = "0.1.0";
        src = sources;
        nativeBuildInputs = nativeTools;
        buildInputs = libraries32;
        meta = {
          description = "King's Field II source port (requires the original Japanese disc)";
          mainProgram = "kings-field-2";
          platforms = [ "i686-linux" ];
        };
      };
      launcher = pkgs.writeShellApplication {
        name = "kings-field-2";
        text = ''
          if [ -z "''${KF_DISC:-}" ] && [ "$#" -eq 0 ]; then
            echo "Set KF_DISC to your King's Field II (Japan) .cue, .bin or .iso image." >&2
            exit 1
          fi
          exec ${unwrapped}/bin/kings-field-2 "$@"
        '';
      };
    in {
      packages.${system} = { default = launcher; unwrapped = unwrapped; };
      apps.${system}.default = {
        type = "app";
        program = "${launcher}/bin/kings-field-2";
        meta.description = "King's Field II source port";
      };
      checks.${system}.native = unwrapped;
      devShells.${system}.default = (pkgs32.mkShell.override { stdenv = pkgs32.clangStdenv; }) {
        packages = nativeTools ++ libraries32 ++ (with pkgs; [ emscripten nodejs ]);
        KF_SDL_SOURCE = "${pkgs.sdl3.src}";
      };
    };
}
