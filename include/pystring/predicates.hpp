#ifndef PYSTRING_PREDICATES_HPP
#define PYSTRING_PREDICATES_HPP

#include <cctype>
#include <cstddef>
#include <string_view>

namespace pystring {

/**
 * @brief Checks if all characters in the string are alphanumeric and there is at least one character.
 */
inline bool isalnum(std::string_view s) noexcept {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are alphabetic and there is at least one character.
 */
inline bool isalpha(std::string_view s) noexcept {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isalpha(c)) return false;
    }
    return true;
}

/**
 * @brief Checks if the string is empty or all characters are ASCII (0..127).
 */
inline bool isascii(std::string_view s) noexcept {
    for (unsigned char c : s) {
        if (c > 127) return false;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are digits and there is at least one character.
 */
inline bool isdigit(std::string_view s) noexcept {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

/**
 * @brief Checks if all characters are decimal digits (0-9) and there is at least one character.
 */
inline bool isdecimal(std::string_view s) noexcept {
    return isdigit(s);
}

/**
 * @brief Checks if all characters are numeric and there is at least one character.
 */
inline bool isnumeric(std::string_view s) noexcept {
    return isdigit(s);
}

/**
 * @brief Checks if all cased characters are lowercase and there is at least one cased character.
 */
inline bool islower(std::string_view s) noexcept {
    if (s.empty()) return false;
    bool has_cased = false;
    for (unsigned char c : s) {
        if (std::isalpha(c)) {
            has_cased = true;
            if (!std::islower(c)) return false;
        }
    }
    return has_cased;
}

/**
 * @brief Checks if all cased characters are uppercase and there is at least one cased character.
 */
inline bool isupper(std::string_view s) noexcept {
    if (s.empty()) return false;
    bool has_cased = false;
    for (unsigned char c : s) {
        if (std::isalpha(c)) {
            has_cased = true;
            if (!std::isupper(c)) return false;
        }
    }
    return has_cased;
}

/**
 * @brief Checks if all characters in the string are whitespace and there is at least one character.
 */
inline bool isspace(std::string_view s) noexcept {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isspace(c)) return false;
    }
    return true;
}

/**
 * @brief Checks if the string is titlecased and there is at least one character.
 */
inline bool istitle(std::string_view s) noexcept {
    if (s.empty()) return false;
    bool cased = false;
    bool previous_is_cased = false;

    for (unsigned char c : s) {
        if (std::isupper(c)) {
            if (previous_is_cased) return false;
            previous_is_cased = true;
            cased = true;
        } else if (std::islower(c)) {
            if (!previous_is_cased) return false;
            previous_is_cased = true;
            cased = true;
        } else {
            previous_is_cased = false;
        }
    }
    return cased;
}

/**
 * @brief Checks if all characters are printable or if the string is empty.
 */
inline bool isprintable(std::string_view s) noexcept {
    for (unsigned char c : s) {
        if (c < 32 || c == 127) return false;
    }
    return true;
}

/**
 * @brief Checks if the string is a valid identifier ([a-zA-Z_][a-zA-Z0-9_]*).
 */
inline bool isidentifier(std::string_view s) noexcept {
    if (s.empty()) return false;
    unsigned char first = static_cast<unsigned char>(s[0]);
    if (!std::isalpha(first) && first != '_') return false;

    for (size_t i = 1; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

/**
 * @brief Checks if haystack contains needle (equivalent to Python's `needle in haystack`).
 */
inline bool contains(std::string_view haystack, std::string_view needle) noexcept {
    return haystack.find(needle) != std::string_view::npos;
}

inline bool contains(std::string_view haystack, char needle) noexcept {
    return haystack.find(needle) != std::string_view::npos;
}

} // namespace pystring

#endif // PYSTRING_PREDICATES_HPP
