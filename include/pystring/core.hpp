#ifndef PYSTRING_CORE_HPP
#define PYSTRING_CORE_HPP

#include "slice.hpp"
#include "predicates.hpp"
#include "search.hpp"
#include "split_join.hpp"
#include "ops.hpp"
#include "format.hpp"
#include "utf8.hpp"
#include "case_conv.hpp"
#include "regex_ops.hpp"
#include "algo.hpp"

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace pystring {

/**
 * @brief Rich Pythonic string class providing Python str methods and syntax in modern C++.
 */
class String {
private:
    std::string m_str;

public:
    // ------------------------------------------------------------------------
    // Constructors & Conversions
    // ------------------------------------------------------------------------
    String() = default;
    String(const String&) = default;
    String(String&&) noexcept = default;
    String& operator=(const String&) = default;
    String& operator=(String&&) noexcept = default;

    String(const char* s) : m_str(s ? s : "") {}
    String(const char* s, size_t count) : m_str(s, count) {}
    String(std::string_view sv) : m_str(sv) {}
    String(const std::string& s) : m_str(s) {}
    String(std::string&& s) noexcept : m_str(std::move(s)) {}
    String(size_t count, char ch) : m_str(count, ch) {}

    operator std::string_view() const noexcept { return m_str; }
    explicit operator std::string() const { return m_str; }

    const std::string& str() const noexcept { return m_str; }
    std::string& str() noexcept { return m_str; }
    const std::string& std_str() const noexcept { return m_str; }
    std::string& std_str() noexcept { return m_str; }
    std::string_view view() const noexcept { return m_str; }
    const char* c_str() const noexcept { return m_str.c_str(); }
    const char* data() const noexcept { return m_str.data(); }
    char* data() noexcept { return m_str.data(); }

    size_t size() const noexcept { return m_str.size(); }
    size_t length() const noexcept { return m_str.length(); }
    bool empty() const noexcept { return m_str.empty(); }
    void clear() noexcept { m_str.clear(); }
    void reserve(size_t n) { m_str.reserve(n); }
    size_t capacity() const noexcept { return m_str.capacity(); }

    // ------------------------------------------------------------------------
    // Iterators
    // ------------------------------------------------------------------------
    auto begin() noexcept { return m_str.begin(); }
    auto end() noexcept { return m_str.end(); }
    auto begin() const noexcept { return m_str.begin(); }
    auto end() const noexcept { return m_str.end(); }
    auto cbegin() const noexcept { return m_str.cbegin(); }
    auto cend() const noexcept { return m_str.cend(); }
    auto rbegin() noexcept { return m_str.rbegin(); }
    auto rend() noexcept { return m_str.rend(); }
    auto rbegin() const noexcept { return m_str.rbegin(); }
    auto rend() const noexcept { return m_str.rend(); }

    // ------------------------------------------------------------------------
    // Python Subscript & Slicing
    // ------------------------------------------------------------------------
    char& operator[](ptrdiff_t idx) {
        if (idx < 0) idx += static_cast<ptrdiff_t>(m_str.size());
        return m_str[static_cast<size_t>(idx)];
    }

    char operator[](ptrdiff_t idx) const {
        if (idx < 0) idx += static_cast<ptrdiff_t>(m_str.size());
        return m_str[static_cast<size_t>(idx)];
    }

    char& at(ptrdiff_t idx) {
        ptrdiff_t original = idx;
        if (idx < 0) idx += static_cast<ptrdiff_t>(m_str.size());
        if (idx < 0 || static_cast<size_t>(idx) >= m_str.size()) {
            throw std::out_of_range("String index out of range: " + std::to_string(original));
        }
        return m_str.at(static_cast<size_t>(idx));
    }

    char at(ptrdiff_t idx) const {
        ptrdiff_t original = idx;
        if (idx < 0) idx += static_cast<ptrdiff_t>(m_str.size());
        if (idx < 0 || static_cast<size_t>(idx) >= m_str.size()) {
            throw std::out_of_range("String index out of range: " + std::to_string(original));
        }
        return m_str.at(static_cast<size_t>(idx));
    }

    String operator[](const Slice& sl) const {
        return String(pystring::slice(m_str, sl));
    }

    String operator()(std::optional<ptrdiff_t> start = std::nullopt,
                      std::optional<ptrdiff_t> stop = std::nullopt,
                      std::optional<ptrdiff_t> step = std::nullopt) const {
        return String(pystring::slice(m_str, Slice(start, stop, step)));
    }

    String slice(std::optional<ptrdiff_t> start = std::nullopt,
                 std::optional<ptrdiff_t> stop = std::nullopt,
                 std::optional<ptrdiff_t> step = std::nullopt) const {
        return String(pystring::slice(m_str, Slice(start, stop, step)));
    }

    String slice(const Slice& sl) const {
        return String(pystring::slice(m_str, sl));
    }

    // ------------------------------------------------------------------------
    // UTF-8 & Unicode Support
    // ------------------------------------------------------------------------
    size_t utf8_len() const noexcept {
        return pystring::utf8_len(m_str);
    }

    bool is_valid_utf8() const noexcept {
        return pystring::is_valid_utf8(m_str);
    }

    String utf8_slice(std::optional<ptrdiff_t> start = std::nullopt,
                      std::optional<ptrdiff_t> stop = std::nullopt,
                      std::optional<ptrdiff_t> step = std::nullopt) const {
        return String(pystring::utf8_slice(m_str, start, stop, step));
    }

    String utf8_slice(const Slice& sl) const {
        return String(pystring::utf8_slice(m_str, sl));
    }

    String utf8_reverse() const {
        return String(pystring::utf8_reverse(m_str));
    }

    std::vector<String> utf8_chars() const {
        auto chars = pystring::utf8_chars(m_str);
        std::vector<String> result;
        result.reserve(chars.size());
        for (auto& c : chars) {
            result.emplace_back(std::move(c));
        }
        return result;
    }

    // ------------------------------------------------------------------------
    // Case Converters (snake_case, camelCase, kebab-case, PascalCase)
    // ------------------------------------------------------------------------
    String to_snake_case() const { return String(pystring::to_snake_case(m_str)); }
    String to_camel_case() const { return String(pystring::to_camel_case(m_str)); }
    String to_kebab_case() const { return String(pystring::to_kebab_case(m_str)); }
    String to_pascal_case() const { return String(pystring::to_pascal_case(m_str)); }

    // ------------------------------------------------------------------------
    // Regex Operations (Python re style)
    // ------------------------------------------------------------------------
    bool matches(std::string_view pattern) const {
        return pystring::matches(m_str, pattern);
    }
    bool search_regex(std::string_view pattern) const {
        return pystring::search_regex(m_str, pattern);
    }
    String replace_regex(std::string_view pattern, std::string_view replacement) const {
        return String(pystring::replace_regex(m_str, pattern, replacement));
    }
    std::vector<String> split_regex(std::string_view pattern) const {
        auto parts = pystring::split_regex(m_str, pattern);
        std::vector<String> result;
        result.reserve(parts.size());
        for (auto& p : parts) result.emplace_back(std::move(p));
        return result;
    }
    std::vector<String> findall(std::string_view pattern) const {
        auto matches_list = pystring::findall(m_str, pattern);
        std::vector<String> result;
        result.reserve(matches_list.size());
        for (auto& m : matches_list) result.emplace_back(std::move(m));
        return result;
    }

    // ------------------------------------------------------------------------
    // Algorithms & Codecs (Levenshtein, Similarity, Base64, Hex)
    // ------------------------------------------------------------------------
    size_t levenshtein(std::string_view other) const {
        return pystring::levenshtein(m_str, other);
    }
    static size_t levenshtein(std::string_view s1, std::string_view s2) {
        return pystring::levenshtein(s1, s2);
    }

    double similarity(std::string_view other) const {
        return pystring::similarity(m_str, other);
    }
    static double similarity(std::string_view s1, std::string_view s2) {
        return pystring::similarity(s1, s2);
    }

    String to_base64() const {
        return String(pystring::to_base64(m_str));
    }
    static String from_base64(std::string_view s) {
        return String(pystring::from_base64(s));
    }

    String to_hex(bool uppercase = false) const {
        return String(pystring::to_hex(m_str, uppercase));
    }
    static String from_hex(std::string_view s) {
        return String(pystring::from_hex(s));
    }

    // ------------------------------------------------------------------------
    // Predicates
    // ------------------------------------------------------------------------
    bool isalnum() const noexcept { return pystring::isalnum(m_str); }
    bool isalpha() const noexcept { return pystring::isalpha(m_str); }
    bool isascii() const noexcept { return pystring::isascii(m_str); }
    bool isdecimal() const noexcept { return pystring::isdecimal(m_str); }
    bool isdigit() const noexcept { return pystring::isdigit(m_str); }
    bool isidentifier() const noexcept { return pystring::isidentifier(m_str); }
    bool islower() const noexcept { return pystring::islower(m_str); }
    bool isnumeric() const noexcept { return pystring::isnumeric(m_str); }
    bool isprintable() const noexcept { return pystring::isprintable(m_str); }
    bool isspace() const noexcept { return pystring::isspace(m_str); }
    bool istitle() const noexcept { return pystring::istitle(m_str); }
    bool isupper() const noexcept { return pystring::isupper(m_str); }

    bool contains(std::string_view needle) const noexcept { return pystring::contains(m_str, needle); }
    bool contains(char needle) const noexcept { return pystring::contains(m_str, needle); }

    // ------------------------------------------------------------------------
    // Search & Inspection
    // ------------------------------------------------------------------------
    ptrdiff_t find(std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::find(m_str, sub, start, end);
    }
    ptrdiff_t find(char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::find(m_str, ch, start, end);
    }

    ptrdiff_t rfind(std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::rfind(m_str, sub, start, end);
    }
    ptrdiff_t rfind(char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::rfind(m_str, ch, start, end);
    }

    size_t index(std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const {
        return pystring::index(m_str, sub, start, end);
    }
    size_t index(char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const {
        return pystring::index(m_str, ch, start, end);
    }

    size_t rindex(std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const {
        return pystring::rindex(m_str, sub, start, end);
    }
    size_t rindex(char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const {
        return pystring::rindex(m_str, ch, start, end);
    }

    size_t count(std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::count(m_str, sub, start, end);
    }
    size_t count(char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::count(m_str, ch, start, end);
    }

    bool startswith(std::string_view prefix, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::startswith(m_str, prefix, start, end);
    }
    bool startswith(std::initializer_list<std::string_view> prefixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::startswith(m_str, prefixes, start, end);
    }

    bool endswith(std::string_view suffix, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::endswith(m_str, suffix, start, end);
    }
    bool endswith(std::initializer_list<std::string_view> suffixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) const noexcept {
        return pystring::endswith(m_str, suffixes, start, end);
    }

    std::tuple<String, String, String> partition(std::string_view sep) const {
        auto [h, m, t] = pystring::partition(m_str, sep);
        return {String(std::move(h)), String(std::move(m)), String(std::move(t))};
    }

    std::tuple<String, String, String> rpartition(std::string_view sep) const {
        auto [h, m, t] = pystring::rpartition(m_str, sep);
        return {String(std::move(h)), String(std::move(m)), String(std::move(t))};
    }

    // ------------------------------------------------------------------------
    // Transformations (Chainable)
    // ------------------------------------------------------------------------
    String capitalize() const { return String(pystring::capitalize(m_str)); }
    String lower() const { return String(pystring::lower(m_str)); }
    String upper() const { return String(pystring::upper(m_str)); }
    String casefold() const { return String(pystring::casefold(m_str)); }
    String swapcase() const { return String(pystring::swapcase(m_str)); }
    String title() const { return String(pystring::title(m_str)); }

    String strip(std::string_view chars = detail::DEFAULT_WHITESPACE) const {
        return String(pystring::strip(m_str, chars));
    }
    String lstrip(std::string_view chars = detail::DEFAULT_WHITESPACE) const {
        return String(pystring::lstrip(m_str, chars));
    }
    String rstrip(std::string_view chars = detail::DEFAULT_WHITESPACE) const {
        return String(pystring::rstrip(m_str, chars));
    }

    String removeprefix(std::string_view prefix) const {
        return String(pystring::removeprefix(m_str, prefix));
    }
    String removesuffix(std::string_view suffix) const {
        return String(pystring::removesuffix(m_str, suffix));
    }

    String replace(std::string_view old_str, std::string_view new_str, ptrdiff_t count = -1) const {
        return String(pystring::replace(m_str, old_str, new_str, count));
    }

    String ljust(size_t width, char fillchar = ' ') const {
        return String(pystring::ljust(m_str, width, fillchar));
    }
    String rjust(size_t width, char fillchar = ' ') const {
        return String(pystring::rjust(m_str, width, fillchar));
    }
    String center(size_t width, char fillchar = ' ') const {
        return String(pystring::center(m_str, width, fillchar));
    }
    String zfill(size_t width) const {
        return String(pystring::zfill(m_str, width));
    }
    String expandtabs(size_t tabsize = 8) const {
        return String(pystring::expandtabs(m_str, tabsize));
    }

    // In-place transforms
    String& strip_inplace(std::string_view chars = detail::DEFAULT_WHITESPACE) {
        m_str = pystring::strip(m_str, chars);
        return *this;
    }
    String& lower_inplace() {
        for (char& c : m_str) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return *this;
    }
    String& upper_inplace() {
        for (char& c : m_str) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return *this;
    }

    // ------------------------------------------------------------------------
    // Split & Join
    // ------------------------------------------------------------------------
    std::vector<String> split(std::string_view sep = "", ptrdiff_t maxsplit = -1) const {
        auto vec = pystring::split(m_str, sep, maxsplit);
        std::vector<String> result;
        result.reserve(vec.size());
        for (auto& s : vec) {
            result.emplace_back(std::move(s));
        }
        return result;
    }

    std::vector<std::string_view> split_view(std::string_view sep = "", ptrdiff_t maxsplit = -1) const {
        return pystring::split_view(m_str, sep, maxsplit);
    }

    std::vector<String> rsplit(std::string_view sep = "", ptrdiff_t maxsplit = -1) const {
        auto vec = pystring::rsplit(m_str, sep, maxsplit);
        std::vector<String> result;
        result.reserve(vec.size());
        for (auto& s : vec) {
            result.emplace_back(std::move(s));
        }
        return result;
    }

    std::vector<std::string_view> rsplit_view(std::string_view sep = "", ptrdiff_t maxsplit = -1) const {
        return pystring::rsplit_view(m_str, sep, maxsplit);
    }

    std::vector<String> splitlines(bool keepends = false) const {
        auto vec = pystring::splitlines(m_str, keepends);
        std::vector<String> result;
        result.reserve(vec.size());
        for (auto& s : vec) {
            result.emplace_back(std::move(s));
        }
        return result;
    }

    template <typename Container>
    String join(const Container& items) const {
        return String(pystring::join(m_str, items));
    }

    template <typename T>
    String join(std::initializer_list<T> items) const {
        return String(pystring::join(m_str, items));
    }

    // ------------------------------------------------------------------------
    // Formatting
    // ------------------------------------------------------------------------
    template <typename... Args>
    String format(const Args&... args) const {
        return String(pystring::format(m_str, args...));
    }

    String format_map(const std::unordered_map<std::string, std::string>& mapping) const {
        return String(pystring::format_map(m_str, mapping));
    }

    String format_map(std::initializer_list<std::pair<std::string, std::string>> mapping) const {
        return String(pystring::format_map(m_str, mapping));
    }

    // ------------------------------------------------------------------------
    // Legacy Helpers
    // ------------------------------------------------------------------------
    String revers() const { return String(pystring::revers(m_str)); }
    String removeVowels() const { return String(pystring::removeVowels(m_str)); }

    // ------------------------------------------------------------------------
    // In-class Mutating Operators
    // ------------------------------------------------------------------------
    String& operator+=(std::string_view rhs) { m_str.append(rhs); return *this; }
    String& operator+=(const char* rhs) { if (rhs) m_str.append(rhs); return *this; }
    String& operator+=(char rhs) { m_str.push_back(rhs); return *this; }

    String operator*(ptrdiff_t count) const {
        return String(pystring::repeat(m_str, count));
    }

    template <typename T>
    String operator%(const T& val) const {
        return String(pystring::format(m_str, val));
    }
};

// ----------------------------------------------------------------------------
// Equality & Comparison Operators
// ----------------------------------------------------------------------------
#define PYSTRING_DEFINE_CMP(OP) \
    inline bool operator OP(const String& lhs, const String& rhs) noexcept { return lhs.view() OP rhs.view(); } \
    inline bool operator OP(const String& lhs, std::string_view rhs) noexcept { return lhs.view() OP rhs; } \
    inline bool operator OP(std::string_view lhs, const String& rhs) noexcept { return lhs OP rhs.view(); } \
    inline bool operator OP(const String& lhs, const char* rhs) noexcept { return lhs.view() OP std::string_view(rhs ? rhs : ""); } \
    inline bool operator OP(const char* lhs, const String& rhs) noexcept { return std::string_view(lhs ? lhs : "") OP rhs.view(); }

PYSTRING_DEFINE_CMP(==)
PYSTRING_DEFINE_CMP(!=)
PYSTRING_DEFINE_CMP(<)
PYSTRING_DEFINE_CMP(<=)
PYSTRING_DEFINE_CMP(>)
PYSTRING_DEFINE_CMP(>=)

#undef PYSTRING_DEFINE_CMP

// ----------------------------------------------------------------------------
// Binary Addition (+)
// ----------------------------------------------------------------------------
inline String operator+(const String& lhs, const String& rhs) {
    String res;
    res.reserve(lhs.size() + rhs.size());
    res += lhs.view();
    res += rhs.view();
    return res;
}
inline String operator+(const String& lhs, std::string_view rhs) {
    String res;
    res.reserve(lhs.size() + rhs.size());
    res += lhs.view();
    res += rhs;
    return res;
}
inline String operator+(std::string_view lhs, const String& rhs) {
    String res;
    res.reserve(lhs.size() + rhs.size());
    res += lhs;
    res += rhs.view();
    return res;
}
inline String operator+(const String& lhs, const char* rhs) {
    String res(lhs);
    res += std::string_view(rhs ? rhs : "");
    return res;
}
inline String operator+(const char* lhs, const String& rhs) {
    String res(lhs ? lhs : "");
    res += rhs.view();
    return res;
}
inline String operator+(const String& lhs, char rhs) {
    String res(lhs);
    res += rhs;
    return res;
}
inline String operator+(char lhs, const String& rhs) {
    String res(1, lhs);
    res += rhs.view();
    return res;
}

// Repetition operator: 3 * s
inline String operator*(ptrdiff_t count, const String& s) {
    return s * count;
}

// Stream I/O operators
inline std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.view();
}

inline std::istream& operator>>(std::istream& is, String& s) {
    return is >> s.std_str();
}

} // namespace pystring

namespace std {
template <>
struct hash<pystring::String> {
    size_t operator()(const pystring::String& s) const noexcept {
        return std::hash<std::string_view>{}(s.view());
    }
};
} // namespace std

#endif // PYSTRING_CORE_HPP
