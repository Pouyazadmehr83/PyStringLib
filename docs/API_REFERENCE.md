# PyStringLib: Complete API Reference

Welcome to the comprehensive API documentation for **PyStringLib**.

All functions and classes live within `namespace pystring`.

---

## Table of Contents

- [The `pystring::String` Class](#the-pystringstring-class)
- [Slicing & Subscripts](#slicing--subscripts)
- [Case Transformations](#case-transformations)
- [Trimming & Stripping](#trimming--stripping)
- [Padding & Alignment](#padding--alignment)
- [Searching & Inspection](#searching--inspection)
- [Splitting & Partitioning](#splitting--partitioning)
- [Joining](#joining)
- [Predicates (`is*`)](#predicates-is)
- [Pythonic Formatting](#pythonic-formatting)
- [Operators & Syntax Overloads](#operators--syntax-overloads)
- [Zero-Copy Views](#zero-copy-views)

---

## The `pystring::String` Class

A rich wrapper around `std::string` providing Python's string interface while maintaining seamless compatibility with `std::string_view` and standard C++ libraries.

```cpp
#include <pystring/pystring.hpp>
using pystring::String;
// Or using Python alias:
using pystring::str;
```

### Constructors
- `String()`: Default empty string.
- `String(const char* s)`: From C-string.
- `String(const char* s, size_t count)`: From buffer with length.
- `String(std::string_view sv)`: From `std::string_view`.
- `String(const std::string& s)`: Copy constructor from `std::string`.
- `String(std::string&& s)`: Move constructor from `std::string`.
- `String(size_t count, char ch)`: Creates repeated character string.

### Interoperability
- `operator std::string_view() const noexcept`: Implicit conversion to `std::string_view`.
- `const std::string& str() const noexcept`: Access underlying `std::string`.
- `const char* c_str() const noexcept`: C-string pointer.
- `size_t size() const noexcept` / `length()`: Length in bytes.
- `bool empty() const noexcept`: Checks if empty.

---

## Slicing & Subscripts

### Python Negative Indexing
- `char operator[](ptrdiff_t index)`
- `char at(ptrdiff_t index)`: Throws `std::out_of_range` on out-of-bounds.
```cpp
String s = "Python";
s[-1];     // 'n'
s[-2];     // 'o'
s.at(-6);  // 'P'
```

### Slicing
Supports all Python slicing semantics including `start`, `stop`, `step`, and negative indices.
- `String slice(optional<ptrdiff_t> start, optional<ptrdiff_t> stop, optional<ptrdiff_t> step = 1) const`
- `String operator()(optional<ptrdiff_t> start, optional<ptrdiff_t> stop, optional<ptrdiff_t> step = 1) const`
- `String operator[](const Slice& sl) const`

```cpp
String s = "Hello, World!";

s(0, 5);                             // "Hello"
s(7, nullopt);                       // "World!"
s(nullopt, 5);                       // "Hello"
s(-6, -1);                           // "World"
s(0, 5, 2);                          // "Hlo"
s(nullopt, nullopt, -1);             // "!dlroW ,olleH" (Full reverse)
s[slice(0, 5)];                      // "Hello"
```

---

## Case Transformations

| Method | Signature | Description | Python Equivalent |
|---|---|---|---|
| `capitalize()` | `String capitalize() const` | First character uppercase, remaining lowercase | `s.capitalize()` |
| `lower()` | `String lower() const` | Converts all cased characters to lowercase | `s.lower()` |
| `upper()` | `String upper() const` | Converts all cased characters to uppercase | `s.upper()` |
| `casefold()` | `String casefold() const` | Aggressive lowercase for caseless comparison | `s.casefold()` |
| `swapcase()` | `String swapcase() const` | Inverts case of each character | `s.swapcase()` |
| `title()` | `String title() const` | Capitalizes words separated by non-alphanumeric chars | `s.title()` |

### In-Place Transformations (Zero-allocation)
- `String& lower_inplace()`
- `String& upper_inplace()`

---

## Trimming & Stripping

| Method | Signature | Description |
|---|---|---|
| `strip(chars = " \t\n\r\f\v")` | `String strip(...) const` | Removes leading and trailing characters |
| `lstrip(chars = " \t\n\r\f\v")` | `String lstrip(...) const` | Removes leading characters |
| `rstrip(chars = " \t\n\r\f\v")` | `String rstrip(...) const` | Removes trailing characters |
| `removeprefix(prefix)` | `String removeprefix(string_view) const` | Strips prefix if present (Python 3.9+) |
| `removesuffix(suffix)` | `String removesuffix(string_view) const` | Strips suffix if present (Python 3.9+) |

---

## Padding & Alignment

| Method | Description | Example |
|---|---|---|
| `center(width, fillchar = ' ')` | Centers string within `width` | `String("hi").center(6, '-') == "--hi--"` |
| `ljust(width, fillchar = ' ')` | Left-justifies string | `String("hi").ljust(5, '.') == "hi..."` |
| `rjust(width, fillchar = ' ')` | Right-justifies string | `String("hi").rjust(5, '.') == "...hi"` |
| `zfill(width)` | Left pads with '0', preserving sign (+/-) | `String("-42").zfill(5) == "-0042"` |
| `expandtabs(tabsize = 8)` | Replaces tabs with spaces preserving tab stops | `String("a\tb").expandtabs(4) == "a   b"` |

---

## Searching & Inspection

| Method | Return | Description |
|---|---|---|
| `find(sub, start=0, end=nullopt)` | `ptrdiff_t` | Lowest index of substring or -1 |
| `rfind(sub, start=0, end=nullopt)` | `ptrdiff_t` | Highest index of substring or -1 |
| `index(sub, start=0, end=nullopt)` | `size_t` | Like find, throws `std::out_of_range` if not found |
| `rindex(sub, start=0, end=nullopt)` | `size_t` | Like rfind, throws `std::out_of_range` if not found |
| `count(sub, start=0, end=nullopt)` | `size_t` | Non-overlapping occurrence count |
| `startswith(prefix, ...)` | `bool` | Checks prefix (also accepts `initializer_list`) |
| `endswith(suffix, ...)` | `bool` | Checks suffix (also accepts `initializer_list`) |
| `contains(sub)` | `bool` | Checks if substring exists (`needle in haystack`) |
| `replace(old, new, count=-1)` | `String` | Replaces occurrences up to `count` times |

---

## Splitting & Partitioning

### `split(sep = "", maxsplit = -1)`
- If `sep` is empty (default), splits by contiguous whitespace, discarding leading and trailing spaces (matches Python `s.split()`).
- If `sep` is non-empty, splits strictly on exact delimiter.
```cpp
String("  a   b  c  ").split();     // ["a", "b", "c"]
String("1,2,3,4").split(",", 2);     // ["1", "2", "3,4"]
```

### `rsplit(sep = "", maxsplit = -1)`
Splits from the right side when `maxsplit >= 0`.
```cpp
String("1,2,3,4").rsplit(",", 2);    // ["1,2", "3", "4"]
```

### `splitlines(keepends = false)`
Splits string at line breaks (`\n`, `\r\n`, `\r`, etc.).

### `partition(sep)` & `rpartition(sep)`
Returns `std::tuple<String, String, String>` containing `{before, sep, after}`.
```cpp
auto [user, at, domain] = String("pouya@example.com").partition("@");
// user == "pouya", at == "@", domain == "example.com"
```

---

## Joining

Joins elements of any container, iterator range, or initializer list using delimiter:
```cpp
std::vector<std::string> fruits = {"apple", "banana", "cherry"};
String(", ").join(fruits);           // "apple, banana, cherry"
String("-").join({"A", "B", "C"});   // "A-B-C"
```

---

## Predicates (`is*`)

All predicates return `false` on empty strings (matching Python).

- `isalnum()`: True if all chars are alphanumeric.
- `isalpha()`: True if all chars are alphabetic.
- `isascii()`: True if empty or all chars <= 127.
- `isdigit()` / `isdecimal()` / `isnumeric()`: True if all chars are digits (0-9).
- `isidentifier()`: True if valid C++/Python identifier (`[a-zA-Z_][a-zA-Z0-9_]*`).
- `islower()`: True if all cased characters are lowercase.
- `isupper()`: True if all cased characters are uppercase.
- `isprintable()`: True if all characters are printable ASCII.
- `isspace()`: True if all characters are whitespace.
- `istitle()`: True if string is titlecased.

---

## Pythonic Formatting

### Positional & Numbered
```cpp
String msg = pystring::format("Hello {}, you have {} tasks.", "Pouya", 3);
// "Hello Pouya, you have 3 tasks."

String s = pystring::format("{0} is {1}, {0} is awesome!", "C++", "fast");
// "C++ is fast, C++ is awesome!"
```

### Named Mapping
```cpp
String s = pystring::format_map("{user} scored {score}", {{"user", "Alice"}, {"score", "100"}});
// "Alice scored 100"
```

---

## Operators & Syntax Overloads

- `String * int` and `int * String`: Repetition (e.g. `String("-") * 20`).
- `s[-1]`: Negative index access.
- `s1 + s2`: Concatenation with `String`, `std::string_view`, `const char*`, `char`.
- `==, !=, <, <=, >, >=`: Complete relational comparisons.
- `std::cout << s`: Stream insertion.
- `std::hash<pystring::String>`: Enabled for `std::unordered_map` and `std::unordered_set`.

---

## Zero-Copy Views

For maximum performance, free functions provide zero-allocation `std::string_view` outputs:
- `pystring::split_view(str_view, sep)`
- `pystring::rsplit_view(str_view, sep)`
- `pystring::splitlines_view(str_view)`
- `pystring::strip_view(str_view)`
- `pystring::lstrip_view(str_view)`
- `pystring::rstrip_view(str_view)`
- `pystring::partition_view(str_view, sep)`
- `pystring::rpartition_view(str_view, sep)`

---

## UTF-8 & Unicode Support

Full Unicode and multi-byte character support (safe for Persian, Arabic, CJK, and Emojis without byte truncation):

| Method | Signature | Description |
|---|---|---|
| `utf8_len()` | `size_t utf8_len() const noexcept` | Returns number of Unicode code points (not raw bytes) |
| `is_valid_utf8()` | `bool is_valid_utf8() const noexcept` | Validates UTF-8 encoding integrity |
| `utf8_slice(start, stop, step)` | `String utf8_slice(...) const` | Slices string by Unicode code points safely |
| `utf8_reverse()` | `String utf8_reverse() const` | Reverses string without corrupting multi-byte letters |
| `utf8_chars()` | `std::vector<String> utf8_chars() const` | Splits string into individual Unicode characters |

```cpp
String fa = "سلام دنیا 🌟";
fa.utf8_len();                 // 11 (instead of 20 bytes!)
fa.utf8_slice(0, 4);           // "سلام"
fa.utf8_reverse();             // "🌟 ایند مالس"
```

---

## Case Converters

Transforms identifier conventions between common programming formats:

| Method | Signature | Example Input | Result |
|---|---|---|---|
| `to_snake_case()` | `String to_snake_case() const` | `"userFirstName"` | `"user_first_name"` |
| `to_camel_case()` | `String to_camel_case() const` | `"user_first_name"` | `"userFirstName"` |
| `to_kebab_case()` | `String to_kebab_case() const` | `"user_first_name"` | `"user-first-name"` |
| `to_pascal_case()` | `String to_pascal_case() const` | `"user_first_name"` | `"UserFirstName"` |

---

## Regex Operations

Python `re`-inspired regular expression tools using the C++ standard library:

| Method | Return | Description |
|---|---|---|
| `matches(pattern)` | `bool` | True if entire string matches regex (like `re.fullmatch`) |
| `search_regex(pattern)` | `bool` | True if regex pattern is found anywhere (like `re.search`) |
| `replace_regex(pattern, repl)` | `String` | Replaces occurrences with replacement pattern (like `re.sub`) |
| `split_regex(pattern)` | `std::vector<String>` | Splits string by regex delimiters (like `re.split`) |
| `findall(pattern)` | `std::vector<String>` | Returns list of all matching substrings (like `re.findall`) |

```cpp
String text = "Contact us at info@example.com or support@domain.org";
auto emails = text.findall(R"([\w.-]+@[\w.-]+\.\w+)");
// ["info@example.com", "support@domain.org"]
```

---

## Algorithms & Codecs

| Method | Description | Example |
|---|---|---|
| `levenshtein(other)` | Computes minimum edit distance | `String::levenshtein("kitten", "sitting") == 3` |
| `similarity(other)` | Computes similarity ratio (0.0 .. 1.0) | `String("hello").similarity("hello") == 1.0` |
| `to_base64()` | Encodes string to Base64 | `String("Hello").to_base64() == "SGVsbG8="` |
| `from_base64(b64)` | Decodes Base64 to string | `String::from_base64("SGVsbG8=") == "Hello"` |
| `to_hex(uppercase=false)` | Encodes string to hexadecimal | `String("Hello").to_hex() == "48656c6c6f"` |
| `from_hex(hex)` | Decodes hexadecimal to string | `String::from_hex("48656c6c6f") == "Hello"` |

---

## Interactive Terminal REPL

PyStringLib provides a built-in interactive Python-like terminal shell:

```bash
make repl
./bin/pystring-repl
```

Supports variable assignment (`s = "text"`), live method evaluation (`s.upper()`, `s.to_snake_case()`, `s[::-1]`), repetition, and syntax inspection in real-time.
