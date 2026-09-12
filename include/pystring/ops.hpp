#ifndef PYSTRING_OPS_HPP
#define PYSTRING_OPS_HPP

#include "slice.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

namespace pystring {

namespace detail {
inline constexpr std::string_view DEFAULT_WHITESPACE = " \t\n\r\f\v";
}

/**
 * @brief Returns a copy of s with its first character capitalized and the rest lowercased.
 */
inline std::string capitalize(std::string_view s) {
    if (s.empty()) return "";
    std::string result(s);
    result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[0])));
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

/**
 * @brief Converts all cased characters to lowercase.
 */
inline std::string lower(std::string_view s) {
    std::string result(s);
    for (char& c : result) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

/**
 * @brief Converts all cased characters to uppercase.
 */
inline std::string upper(std::string_view s) {
    std::string result(s);
    for (char& c : result) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return result;
}

/**
 * @brief Returns a casefolded copy of the string (suitable for case-insensitive matching).
 */
inline std::string casefold(std::string_view s) {
    return lower(s);
}

/**
 * @brief Swaps case: uppercase becomes lowercase and vice versa.
 */
inline std::string swapcase(std::string_view s) {
    std::string result(s);
    for (char& c : result) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isupper(uc)) {
            c = static_cast<char>(std::tolower(uc));
        } else if (std::islower(uc)) {
            c = static_cast<char>(std::toupper(uc));
        }
    }
    return result;
}

/**
 * @brief Converts the string to title case.
 */
inline std::string title(std::string_view s) {
    std::string result(s);
    bool prev_is_cased = false;

    for (char& c : result) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalpha(uc)) {
            if (!prev_is_cased) {
                c = static_cast<char>(std::toupper(uc));
                prev_is_cased = true;
            } else {
                c = static_cast<char>(std::tolower(uc));
            }
        } else {
            prev_is_cased = false;
        }
    }
    return result;
}

/**
 * @brief Zero-copy leading character strip.
 */
inline std::string_view lstrip_view(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) noexcept {
    size_t start = s.find_first_not_of(chars);
    if (start == std::string_view::npos) {
        return "";
    }
    return s.substr(start);
}

/**
 * @brief Returns a copy with leading characters removed.
 */
inline std::string lstrip(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) {
    return std::string(lstrip_view(s, chars));
}

/**
 * @brief Zero-copy trailing character strip.
 */
inline std::string_view rstrip_view(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) noexcept {
    size_t end = s.find_last_not_of(chars);
    if (end == std::string_view::npos) {
        return "";
    }
    return s.substr(0, end + 1);
}

/**
 * @brief Returns a copy with trailing characters removed.
 */
inline std::string rstrip(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) {
    return std::string(rstrip_view(s, chars));
}

/**
 * @brief Zero-copy leading and trailing character strip.
 */
inline std::string_view strip_view(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) noexcept {
    return rstrip_view(lstrip_view(s, chars), chars);
}

/**
 * @brief Returns a copy with leading and trailing characters removed.
 */
inline std::string strip(std::string_view s, std::string_view chars = detail::DEFAULT_WHITESPACE) {
    return std::string(strip_view(s, chars));
}

/**
 * @brief Removes prefix if s starts with prefix (Python 3.9+ removeprefix).
 */
inline std::string_view removeprefix_view(std::string_view s, std::string_view prefix) noexcept {
    if (s.size() >= prefix.size() && s.substr(0, prefix.size()) == prefix) {
        return s.substr(prefix.size());
    }
    return s;
}

inline std::string removeprefix(std::string_view s, std::string_view prefix) {
    return std::string(removeprefix_view(s, prefix));
}

/**
 * @brief Removes suffix if s ends with suffix (Python 3.9+ removesuffix).
 */
inline std::string_view removesuffix_view(std::string_view s, std::string_view suffix) noexcept {
    if (s.size() >= suffix.size() && s.substr(s.size() - suffix.size()) == suffix) {
        return s.substr(0, s.size() - suffix.size());
    }
    return s;
}

inline std::string removesuffix(std::string_view s, std::string_view suffix) {
    return std::string(removesuffix_view(s, suffix));
}

/**
 * @brief Left-justifies string in a field of given width using fillchar.
 */
inline std::string ljust(std::string_view s, size_t width, char fillchar = ' ') {
    if (s.size() >= width) {
        return std::string(s);
    }
    std::string result(s);
    result.append(width - s.size(), fillchar);
    return result;
}

/**
 * @brief Right-justifies string in a field of given width using fillchar.
 */
inline std::string rjust(std::string_view s, size_t width, char fillchar = ' ') {
    if (s.size() >= width) {
        return std::string(s);
    }
    std::string result(width - s.size(), fillchar);
    result.append(s);
    return result;
}

/**
 * @brief Centers string in a field of given width using fillchar.
 */
inline std::string center(std::string_view s, size_t width, char fillchar = ' ') {
    if (s.size() >= width) {
        return std::string(s);
    }
    size_t total_pad = width - s.size();
    size_t left_pad = total_pad / 2;
    size_t right_pad = total_pad - left_pad;

    std::string result;
    result.reserve(width);
    result.append(left_pad, fillchar);
    result.append(s);
    result.append(right_pad, fillchar);
    return result;
}

/**
 * @brief Pads string with leading zeros. Preserves leading '+' or '-'.
 */
inline std::string zfill(std::string_view s, size_t width) {
    if (s.size() >= width) {
        return std::string(s);
    }
    size_t pad = width - s.size();
    std::string result;
    result.reserve(width);

    if (!s.empty() && (s[0] == '+' || s[0] == '-')) {
        result.push_back(s[0]);
        result.append(pad, '0');
        result.append(s.substr(1));
    } else {
        result.append(pad, '0');
        result.append(s);
    }
    return result;
}

/**
 * @brief Expands tab characters (\t) using given tabsize.
 */
inline std::string expandtabs(std::string_view s, size_t tabsize = 8) {
    std::string result;
    result.reserve(s.size() * 2);
    size_t col = 0;

    for (char c : s) {
        if (c == '\t') {
            size_t spaces = (tabsize == 0) ? 0 : (tabsize - (col % tabsize));
            result.append(spaces, ' ');
            col += spaces;
        } else {
            result.push_back(c);
            if (c == '\n' || c == '\r') {
                col = 0;
            } else {
                ++col;
            }
        }
    }
    return result;
}

/**
 * @brief Replaces occurrences of old_str with new_str up to count times.
 */
inline std::string replace(std::string_view s, std::string_view old_str, std::string_view new_str, ptrdiff_t count = -1) {
    if (count == 0) {
        return std::string(s);
    }

    if (old_str.empty()) {
        std::string result;
        result.reserve(s.size() + (s.size() + 1) * new_str.size());
        ptrdiff_t replacements = 0;

        for (size_t i = 0; i < s.size(); ++i) {
            if (count < 0 || replacements < count) {
                result.append(new_str);
                ++replacements;
            }
            result.push_back(s[i]);
        }
        if (count < 0 || replacements < count) {
            result.append(new_str);
        }
        return result;
    }

    std::string result;
    result.reserve(s.size());
    size_t start = 0;
    ptrdiff_t replacements = 0;

    while (count < 0 || replacements < count) {
        size_t pos = s.find(old_str, start);
        if (pos == std::string_view::npos) {
            break;
        }
        result.append(s.data() + start, pos - start);
        result.append(new_str);
        start = pos + old_str.size();
        ++replacements;
    }
    result.append(s.data() + start, s.size() - start);
    return result;
}

/**
 * @brief Python-compliant string slicing with step, negative index, and reverse stepping.
 */
inline std::string slice(std::string_view s, const Slice& sl) {
    auto [start_idx, stop_idx, step, len] = sl.compute(s.size());
    if (len == 0) {
        return "";
    }

    std::string result;
    result.reserve(len);

    if (step > 0) {
        for (ptrdiff_t i = start_idx; i < stop_idx && static_cast<size_t>(i) < s.size(); i += step) {
            result.push_back(s[static_cast<size_t>(i)]);
        }
    } else {
        for (ptrdiff_t i = start_idx; i > stop_idx && i >= 0; i += step) {
            result.push_back(s[static_cast<size_t>(i)]);
        }
    }
    return result;
}

inline std::string slice(std::string_view s, 
                         std::optional<ptrdiff_t> start, 
                         std::optional<ptrdiff_t> stop, 
                         std::optional<ptrdiff_t> step = std::nullopt) {
    return slice(s, Slice(start, stop, step));
}

/**
 * @brief Repeats a string n times (mimics Python `s * n`).
 */
inline std::string repeat(std::string_view s, ptrdiff_t count) {
    if (count <= 0 || s.empty()) {
        return "";
    }
    std::string result;
    result.reserve(s.size() * static_cast<size_t>(count));
    for (ptrdiff_t i = 0; i < count; ++i) {
        result.append(s);
    }
    return result;
}

/**
 * @brief Legacy method: reverses a string.
 */
inline std::string revers(std::string_view s) {
    return slice(s, Slice(std::nullopt, std::nullopt, -1));
}

/**
 * @brief Legacy method: removes vowels (a, e, i, o, u) from string.
 */
inline std::string removeVowels(std::string_view s) {
    std::string result;
    result.reserve(s.size());
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc != 'a' && uc != 'e' && uc != 'i' && uc != 'o' && uc != 'u' &&
            uc != 'A' && uc != 'E' && uc != 'I' && uc != 'O' && uc != 'U') {
            result.push_back(c);
        }
    }
    return result;
}

} // namespace pystring

#endif // PYSTRING_OPS_HPP
