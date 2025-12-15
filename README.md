# PyStringLib

PyStringLib — a C++ library implementing Python-like string utilities.

## Overview

PyStringLib provides a set of string utilities inspired by Python's string API, implemented in modern C++ for performance and embeddability. This repo is primarily C++.

## Contents

- Source code (src/)
- Headers (include/)
- CMake configuration (CMakeLists.txt)
- Examples (examples/) and tests (tests/) if present

## Requirements

- C++17-compatible compiler (g++, clang, MSVC)
- CMake 3.10+
- Git
- Optional: Conan/vcpkg for dependency management

## Build (CMake — exact steps)

1. Clone:
   git clone https://github.com/Pouyazadmehr83/PyStringLib.git
   cd PyStringLib

2. Create build directory and configure:
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release

3. Build:
   cmake --build . --config Release

4. (Optional) Run tests:
   ctest --output-on-failure

## Build with g++ (single-file example)

g++ -std=c++17 -O2 -Iinclude -o example examples/example.cpp src/pystringlib.cpp

Adjust file names to the actual source structure.

## Usage

- Link the compiled library to your project or include header-only utilities.
- Example (pseudo):
  #include "pystringlib/pystring.h"
  using namespace pystring;
  auto s = pystring::strip("  hello  ");

## Packaging & Installation

- Optionally create a package with CPack or provide install targets via CMake:
  cmake --install . --prefix /usr/local

## Contributing

- Follow C++17 idioms, add tests for new behavior, and provide clear API documentation.
- Raise PRs with test coverage and examples.

## License & Contact

Add a LICENSE file (recommended).
Author: Pouyazadmehr83
