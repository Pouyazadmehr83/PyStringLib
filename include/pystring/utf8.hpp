#ifndef PYSTRING_UTF8_HPP
#define PYSTRING_UTF8_HPP

#include "slice.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace pystring {

namespace detail {

/**
 * @brief Returns byte length of UTF-8 code point from its leading byte (1..4), or 1 if invalid.
 */
inline size_t utf8_codepoint_length(unsigned char lead) noexcept {
    if ((lead & 0x80) == 0x00) return 1;        // 0xxxxxxx: 1 byte (ASCII)
    if ((lead & 0xE0) == 0xC0) return 2;        // 110xxxxx: 2 bytes
    if ((lead & 0xF0) == 0xE0) return 3;        // 1110xxxx: 3 bytes
    if ((lead & 0xF8) == 0xF0) return 4;        // 11110xxx: 4 bytes
    return 1; // invalid or continuation byte, step by 1
}

} // namespace detail

/**
 * @brief Returns true if the string is valid UTF-8.
 */
inline bool is_valid_utf8(std::string_view s) noexcept {
    size_t i = 0;
    size_t n = s.size();

    while (i < n) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c <= 0x7F) {
            ++i;
        } else if ((c & 0xE0) == 0xC0) {
            if (i + 1 >= n) return false;
            if ((static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80) return false;
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            if (i + 2 >= n) return false;
            if ((static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80) return false;
            if ((static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80) return false;
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            if (i + 3 >= n) return false;
            if ((static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80) return false;
            if ((static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80) return false;
            if ((static_cast<unsigned char>(s[i + 3]) & 0xC0) != 0x80) return false;
            i += 4;
        } else {
            return false;
        }
    }
    return true;
}

/**
 * @brief Returns the number of Unicode code points in s (not raw bytes).
 * Supports Persian, Arabic, CJK, Emojis, and multi-byte characters.
 */
inline size_t utf8_len(std::string_view s) noexcept {
    size_t count = 0;
    for (unsigned char c : s) {
        // Count bytes that are NOT continuation bytes (10xxxxxx)
        if ((c & 0xC0) != 0x80) {
            ++count;
        }
    }
    return count;
}

/**
 * @brief Extracts individual Unicode code points as string views.
 */
inline std::vector<std::string_view> utf8_chars_view(std::string_view s) {
    std::vector<std::string_view> result;
    size_t i = 0;
    size_t n = s.size();

    while (i < n) {
        unsigned char lead = static_cast<unsigned char>(s[i]);
        size_t cp_len = detail::utf8_codepoint_length(lead);
        if (i + cp_len > n) {
            cp_len = n - i;
        }
        result.push_back(s.substr(i, cp_len));
        i += cp_len;
    }
    return result;
}

/**
 * @brief Extracts individual Unicode code points as std::vector<std::string>.
 */
inline std::vector<std::string> utf8_chars(std::string_view s) {
    auto views = utf8_chars_view(s);
    std::vector<std::string> result;
    result.reserve(views.size());
    for (auto v : views) {
        result.emplace_back(v);
    }
    return result;
}

/**
 * @brief Slices a UTF-8 string by Unicode code points (safe for Persian, Arabic, and Emojis).
 */
inline std::string utf8_slice(std::string_view s, const Slice& sl) {
    auto chars = utf8_chars_view(s);
    auto [start_idx, stop_idx, step, len] = sl.compute(chars.size());
    if (len == 0) {
        return "";
    }

    std::string result;
    result.reserve(s.size());

    if (step > 0) {
        for (ptrdiff_t i = start_idx; i < stop_idx && static_cast<size_t>(i) < chars.size(); i += step) {
            result.append(chars[static_cast<size_t>(i)]);
        }
    } else {
        for (ptrdiff_t i = start_idx; i > stop_idx && i >= 0; i += step) {
            result.append(chars[static_cast<size_t>(i)]);
        }
    }
    return result;
}

inline std::string utf8_slice(std::string_view s, 
                              std::optional<ptrdiff_t> start = std::nullopt,
                              std::optional<ptrdiff_t> stop = std::nullopt,
                              std::optional<ptrdiff_t> step = std::nullopt) {
    return utf8_slice(s, Slice(start, stop, step));
}

/**
 * @brief Reverses a UTF-8 string by Unicode code points (does not corrupt multi-byte letters).
 */
inline std::string utf8_reverse(std::string_view s) {
    return utf8_slice(s, Slice(std::nullopt, std::nullopt, -1));
}

} // namespace pystring

#endif // PYSTRING_UTF8_HPP
