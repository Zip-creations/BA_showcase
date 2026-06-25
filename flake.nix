{
  description = "BA showcase project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

    testAuditor = {
      url = "github:Zip-creations/testAuditor";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, testAuditor }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
    in {
      packages.${system} = {
        hello = pkgs.hello;
        default = pkgs.hello;
      };

      devShells.${system}.default = pkgs.mkShell {
        buildInputs = [
          (pkgs.python3.withPackages (ps: [
            ps.pytest
            ps.pytest-tap
            ps.pytest-json-report
          ]))

          testAuditor.packages.${system}.default
        ];
      };
    };
}
