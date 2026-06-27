{
  description = "A flake for u8c cli for char.social";

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0"; # stable Nixpkgs

  outputs =
    { self, ... }@inputs:

    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        inputs.nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import inputs.nixpkgs { inherit system; };
          }
        );
    in
    {
      packages = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "u8c";
            version = builtins.substring 0 10 self.rev;
            src = ./.;
            buildInputs = with pkgs; [
              clang
              gnumake
              stdenv
            ];

            buildPhase = ''
              mkdir -p build
              make release -j
            '';

            installPhase = ''
              runHook preInstall
              mkdir -p $out/bin
              cp build/release/u8c $out/bin
              runHook postInstall
            '';
          };
        }
      );
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default =
            pkgs.mkShell.override
              {
                # Override stdenv in order to change compiler:
                # stdenv = pkgs.clangStdenv;
              }
              {
                shellHook = ''
                  alias mka='make all'
                  alias mkg='make debug'
                  alias mkr='make release'
                  alias mkc='make clean'
                  alias mkl='make list'
                  alias mkh='make help'
                '';
                packages =
                  with pkgs;
                  [
                    clang-tools
                    clang
                    lldb
                    gnumake
                    codespell
                    cppcheck
                    doxygen
                  ]
                  ++ (if stdenv.hostPlatform.system == "aarch64-darwin" then [ ] else [ gdb ]);
              };
        }
      );
    };
}
