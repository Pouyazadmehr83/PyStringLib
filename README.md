<p align="center">
  <img src="assets/banner.svg" alt="PyStringLib - Pythonic String Utilities for Modern C++" width="100%">
</p>

<p align="center">
  <a href="https://github.com/Pouyazadmehr83/PyStringLib/actions"><img src="https://img.shields.io/badge/build-passing-brightgreen?style=flat-square&logo=githubactions" alt="Build Status"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square" alt="License"></a>
  <img src="https://img.shields.io/badge/C%2B%2B-17%20%7C%2020%20%7C%2023-purple?style=flat-square&logo=cplusplus" alt="C++ Standard">
  <img src="https://img.shields.io/badge/library-header--only-orange?style=flat-square" alt="Header Only">
  <img src="https://img.shields.io/badge/tests-147%20passed-success?style=flat-square" alt="Tests">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=flat-square" alt="Platform">
  <a href="docs/README_FA.md"><img src="https://img.shields.io/badge/%D8%B1%D8%A7%D9%87%D9%86%D9%85%D8%A7%DB%8C%20%D9%81%D8%A7%D8%B1%D8%B3%DB%8C-Farsi-green?style=flat-square" alt="Persian Docs"></a>
</p>

<p align="center">
  <b>Python's beloved string manipulation superpowers brought into Modern C++.</b><br>
  Header-only • Zero-overhead string views • Pythonic slicing &amp; negative indexing • UTF-8 Unicode • Interactive REPL • 5.3M+ ops/sec.
</p>

<p align="center">
  <a href="#quickstart">Quickstart</a> •
  <a href="#interactive-terminal-repl">Interactive REPL</a> •
  <a href="#feature-highlights">Features</a> •
  <a href="#pythonic-slicing">Slicing Model</a> •
  <a href="#method-matrix">Method Matrix</a> •
  <a href="#benchmarks">Benchmarks</a> •
  <a href="docs/API_REFERENCE.md">Full API Reference</a> •
  <a href="docs/README_FA.md">راهنمای فارسی</a>
</p>

---

## The Pitch: Modern C++ Meets Python's Elegance

Ever spent 30 lines in C++ writing what takes 1 line in Python? 

```cpp
// ❌ Standard C++ boilerplate to clean, split, and reverse a string:
std::string raw = "  apple,banana,orange  ";
raw.erase(0, raw.find_first_not_of(" \t\n\r"));
raw.erase(raw.find_last_not_of(" \t\n\r") + 1);
std::vector<std::string> tokens;
size_t pos = 0, next;
while ((next = raw.find(',', pos)) != std::string::npos) {
    tokens.push_back(raw.substr(pos, next - pos));
    pos = next + 1;
}
tokens.push_back(raw.substr(pos));
std::reverse(tokens.begin(), tokens.end());

// ✨ With PyStringLib:
using namespace pystring;
auto tokens = String("  apple,banana,orange  ").strip().split(",");
// Slicing backwards:
auto reversed = String("hello world")(nullopt, nullopt, -1); // "dlrow olleh"
```

---

## Feature Highlights

- 🪶 **Single-Header Option:** Drop `single_include/pystring.hpp` into any project and start hacking instantly.
- ⚡ **Zero-Copy Views:** High-performance functions (`split_view`, `strip_view`, `slice_view`, `partition_view`) powered by `std::string_view` for zero heap allocations.
- 🔪 **Full Python Slicing Engine:** Exact CPython `[start:stop:step]` semantics, supporting negative steps, reverse strides, and negative bounds.
- 🌐 **UTF-8 & Unicode Ready:** Safe slicing and character count for Persian, Arabic, CJK, and Emojis (`utf8_len`, `utf8_slice`, `utf8_reverse`).
- 🔄 **Case Converters:** Instant transformation between `snake_case`, `camelCase`, `kebab-case`, and `PascalCase`.
- 🔍 **Regex Powered:** Python `re`-like regex matching, replacing, splitting, and `findall`.
- 🎯 **Fuzzy Matching:** Levenshtein edit distance and similarity ratio (`s.similarity("target")`).
- 💻 **Interactive REPL:** Built-in terminal playground to evaluate Pythonic string expressions live without compiling!
- 🛡️ **Extensively Tested:** 147 comprehensive unit tests covering edge cases, unicode, regex, and negative strides.

---

## Interactive Terminal REPL

PyStringLib includes a built-in interactive shell where you can experiment with strings live in your terminal:

```bash
make repl
./bin/pystring-repl
```

```text
  ____        ____  _        _             _     _ _     
 |  _ \ _   _/ ___|| |_ _ __(_)_ __   __ _| |   (_) |__  
 | |_) | | | \___ \| __| '__| | '_ \ / _` | |   | | '_ \ 
 |  __/| |_| |___) | |_| |  | | | | | (_| | |___| | |_) |
 |_|    \__, |____/ \__|_|  |_|_| |_|\__, |_____|_|_.__/ 
        |___/                        |___/               
  Pythonic String Manipulation for Modern C++ (Interactive REPL)

>>> s = "  Hello, Modern C++ World!  "
>>> s.strip().to_snake_case()
=> "hello_modern_c++_world"
>>> s[::-1]
=> "  !dlroW ++C nredoM ,olleH  "
>>> fa = "سلام دنیا 🌟"
>>> fa.utf8_len()
=> 11 code points
>>> s.similarity("Hello World")
=> 0.392857
```

---

## Quickstart

### 1. Header-Only Drop-in (Fastest)

Download [`single_include/pystring.hpp`](single_include/pystring.hpp) into your project include path:

```cpp
#include "pystring.hpp"
#include <iostream>

int main() {
    using namespace pystring;

    String s = "  hello world!  ";
    
    // Method chaining
    std::cout << s.strip().upper() << "\n";     // "HELLO WORLD!"

    // Python-style slicing
    std::cout << s(2, 7) << "\n";               // "hello"
    std::cout << s[-3] << "\n";                 // 'd' (negative indexing!)

    // Split and join
    auto words = String("c++,python,rust").split(",");
    std::cout << String(" | ").join(words) << "\n"; // "c++ | python | rust"

    // String repetition
    std::cout << (String("=") * 40) << "\n";
}
```

### 2. CMake FetchContent

Add PyStringLib directly to your `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
    pystringlib
    GIT_REPOSITORY https://github.com/Pouyazadmehr83/PyStringLib.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(pystringlib)

target_link_libraries(your_target PRIVATE PyStringLib::pystring)
```

---

## Pythonic Slicing

PyStringLib implements the complete CPython `PySlice_GetIndicesEx` algorithm in pure C++:

<p align="center">
  <img src="assets/slicing_diagram.svg" alt="PyStringLib Slicing Diagram" width="100%">
</p>

```cpp
String s = "0123456789";

// Standard Range [start, stop)
s(2, 6);                        // "2345"
s(nullopt, 4);                  // "0123"
s(6, nullopt);                  // "6789"

// Strides (step)
s(0, 10, 2);                    // "02468"
s(1, 10, 2);                    // "13579"

// Negative indices & Reverse Strides (s[::-1])
s(-4, -1);                      // "678"
s(nullopt, nullopt, -1);        // "9876543210"
s(8, 2, -2);                    // "864"

// Object slice syntax
s[slice(0, 5)];                 // "01234"
```

---

## Method Matrix

PyStringLib implements Python's complete `str` standard library API:

| Category | Methods |
|---|---|
| **Case Mapping** | `capitalize()`, `casefold()`, `lower()`, `upper()`, `swapcase()`, `title()` |
| **Case Converters** | `to_snake_case()`, `to_camel_case()`, `to_kebab_case()`, `to_pascal_case()` |
| **UTF-8 & Unicode** | `utf8_len()`, `utf8_slice()`, `utf8_reverse()`, `utf8_chars()`, `is_valid_utf8()` |
| **Regex (Python re)**| `matches()`, `search_regex()`, `replace_regex()`, `split_regex()`, `findall()` |
| **Codecs & Similarity**| `levenshtein()`, `similarity()`, `to_base64()`, `from_base64()`, `to_hex()`, `from_hex()` |
| **Stripping** | `strip()`, `lstrip()`, `rstrip()`, `removeprefix()`, `removesuffix()` |
| **Splitting** | `split()`, `rsplit()`, `splitlines()`, `partition()`, `rpartition()` |
| **Joining** | `join(container)`, `join(begin, end)`, `join(initializer_list)` |
| **Searching** | `find()`, `rfind()`, `index()`, `rindex()`, `count()`, `contains()` |
| **Prefix / Suffix** | `startswith(prefix)`, `endswith(suffix)` (accepts initializer lists) |
| **Formatting** | `format(...)`, `format_map(...)`, `operator%` |
| **Padding** | `center()`, `ljust()`, `rjust()`, `zfill()`, `expandtabs()` |
| **Predicates** | `isalnum()`, `isalpha()`, `isascii()`, `isdigit()`, `isdecimal()`, `isnumeric()`, `isidentifier()`, `islower()`, `isupper()`, `isprintable()`, `isspace()`, `istitle()` |
| **Zero-Copy Views** | `split_view()`, `rsplit_view()`, `splitlines_view()`, `strip_view()`, `lstrip_view()`, `rstrip_view()`, `partition_view()`, `rpartition_view()` |

👉 Check out the [Complete API Reference](docs/API_REFERENCE.md) for full signatures and complexity guarantees.

---

## Benchmarks

Measured on Ubuntu Linux x86_64, GCC 13.3, `-O3`:

| Operation | Throughput | Latency / Allocations |
|---|---|---|
| **Pythonic Slicing (`s(10, 40, 2)`)** | **5,325,000+ ops/sec** | ~0.18 µs per slice |
| **Zero-Copy Token Split (`split_view`)** | **2,043,000+ ops/sec** | **0 heap allocations** |
| **String Class (`strip + split`)** | **1,249,000+ ops/sec** | Pure C++ cache efficiency |
| **Templated Container Join** | **1,187,000+ ops/sec** | Single reserve allocation |

Run the benchmark on your own machine:
```bash
make benchmark
```

---

## Building & Testing

### Using Make (Zero CMake required)
```bash
git clone https://github.com/Pouyazadmehr83/PyStringLib.git
cd PyStringLib

make test        # Builds and runs all 147 unit tests
make repl        # Builds the interactive terminal shell (./bin/pystring-repl)
make examples    # Compiles showcase executables
make benchmark   # Runs performance benchmark
```

### Using CMake
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Project Structure

```
PyStringLib/
├── assets/                  # SVG diagrams and banners
│   ├── banner.svg
│   └── slicing_diagram.svg
├── docs/                    # Detailed multi-language documentation
│   ├── API_REFERENCE.md     # Full English API specification
│   └── README_FA.md         # کامل‌ترین راهنمای فارسی
├── examples/                # Runnable tutorials and showcases
│   ├── basic_usage.cpp
│   ├── slicing_demo.cpp
│   ├── chaining_demo.cpp
│   └── benchmark.cpp
├── include/                 # Modular header includes
│   └── pystring/
│       ├── core.hpp
│       ├── format.hpp
│       ├── ops.hpp
│       ├── predicates.hpp
│       ├── pystring.hpp
│       ├── search.hpp
│       ├── slice.hpp
│       └── split_join.hpp
├── single_include/          # Amalgamated single-header
│   └── pystring.hpp
├── tests/                   # Comprehensive unit tests
│   └── test_all.cpp
├── CMakeLists.txt           # Modern CMake configuration
├── Makefile                 # Instant Make build
└── LICENSE                  # MIT License
```

---

## Contributing

Contributions, feedback, and bug reports are enthusiastically welcomed!
1. Fork the repo.
2. Create a feature branch (`git checkout -b feature/cool-feature`).
3. Add tests in `tests/test_all.cpp`.
4. Run `python scripts/amalgamate.py` to regenerate the single header.
5. Open a Pull Request!

---

## Author & License

Created by **Pouya Zadmehr** ([@Pouyazadmehr83](https://github.com/Pouyazadmehr83)).

Licensed under the **MIT License** — feel free to use it in personal, academic, and commercial software!

⭐ If you find this library useful, please **give it a star on GitHub**!
