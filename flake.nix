{
  description = "PlatformIO ESP32 devshell for NixOS";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";  # vagy stable, ha stabilabbra van szükséged
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = [
            pkgs.platformio
            pkgs.stdenv.cc.cc.lib  # C/C++ dynamic linker
            pkgs.zlib
            pkgs.ncurses5
          ];

          shellHook = ''
            echo "Welcome to PlatformIO devshell on NixOS 🛠️"
            export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib:$LD_LIBRARY_PATH
          '';
        };
      }
    );
}

