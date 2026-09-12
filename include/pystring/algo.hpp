#ifndef PYSTRING_ALGO_HPP
#define PYSTRING_ALGO_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace pystring {

/**
 * @brief Computes Levenshtein edit distance between two strings (insertions, deletions, substitutions).
 * Space complexity: O(min(m, n)).
 */
inline size_t levenshtein(std::string_view s1, std::string_view s2) {
    if (s1 == s2) return 0;
    if (s1.empty()) return s2.size();
    if (s2.empty()) return s1.size();

    if (s1.size() < s2.size()) {
        std::swap(s1, s2);
    }

    // s2 is shorter or equal length
    std::vector<size_t> prev(s2.size() + 1);
    std::vector<size_t> curr(s2.size() + 1);

    for (size_t j = 0; j <= s2.size(); ++j) {
        prev[j] = j;
    }

    for (size_t i = 0; i < s1.size(); ++i) {
        curr[0] = i + 1;
        for (size_t j = 0; j < s2.size(); ++j) {
            size_t cost = (s1[i] == s2[j]) ? 0 : 1;
            curr[j + 1] = std::min({
                curr[j] + 1,        // insertion
                prev[j + 1] + 1,    // deletion
                prev[j] + cost      // substitution
            });
        }
        prev = curr;
    }

    return prev[s2.size()];
}

/**
 * @brief Computes Levenshtein similarity ratio between 0.0 (completely different) and 1.0 (identical).
 */
inline double similarity(std::string_view s1, std::string_view s2) {
    if (s1 == s2) return 1.0;
    size_t max_len = std::max(s1.size(), s2.size());
    if (max_len == 0) return 1.0;

    size_t dist = levenshtein(s1, s2);
    return 1.0 - (static_cast<double>(dist) / static_cast<double>(max_len));
}

/**
 * @brief Encodes string to Base64.
 */
inline std::string to_base64(std::string_view s) {
    static constexpr char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve(((s.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i < s.size()) {
        size_t remaining = s.size() - i;
        uint32_t octet_a = static_cast<unsigned char>(s[i++]);
        uint32_t octet_b = (remaining > 1) ? static_cast<unsigned char>(s[i++]) : 0;
        uint32_t octet_c = (remaining > 2) ? static_cast<unsigned char>(s[i++]) : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        result.push_back(table[(triple >> 18) & 0x3F]);
        result.push_back(table[(triple >> 12) & 0x3F]);
        result.push_back((remaining > 1) ? table[(triple >> 6) & 0x3F] : '=');
        result.push_back((remaining > 2) ? table[triple & 0x3F] : '=');
    }
    return result;
}

/**
 * @brief Decodes Base64 string.
 */
inline std::string from_base64(std::string_view s) {
    static const auto decode_char = [](char c) -> int {
        if (c >= 'A' && c <= 'Z') return c - 'A';
        if (c >= 'a' && c <= 'z') return c - 'a' + 26;
        if (c >= '0' && c <= '9') return c - '0' + 52;
        if (c == '+') return 62;
        if (c == '/') return 63;
        return -1;
    };

    std::string result;
    result.reserve((s.size() * 3) / 4);

    uint32_t buf = 0;
    int bits = 0;
    for (char c : s) {
        if (c == '=') break;
        int val = decode_char(c);
        if (val < 0) continue; // skip invalid / whitespace
        buf = (buf << 6) | static_cast<uint32_t>(val);
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            result.push_back(static_cast<char>((buf >> bits) & 0xFF));
        }
    }
    return result;
}

/**
 * @brief Converts binary / ASCII string to hexadecimal representation.
 */
inline std::string to_hex(std::string_view s, bool uppercase = false) {
    static constexpr char hex_lower[] = "0123456789abcdef";
    static constexpr char hex_upper[] = "0123456789ABCDEF";
    const char* hex_digits = uppercase ? hex_upper : hex_lower;

    std::string result;
    result.reserve(s.size() * 2);
    for (unsigned char c : s) {
        result.push_back(hex_digits[(c >> 4) & 0x0F]);
        result.push_back(hex_digits[c & 0x0F]);
    }
    return result;
}

/**
 * @brief Decodes hexadecimal string back to original string.
 */
inline std::string from_hex(std::string_view s) {
    auto hex_val = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };

    std::string result;
    result.reserve(s.size() / 2);

    for (size_t i = 0; i + 1 < s.size(); i += 2) {
        int h1 = hex_val(s[i]);
        int h2 = hex_val(s[i + 1]);
        if (h1 >= 0 && h2 >= 0) {
            result.push_back(static_cast<char>((h1 << 4) | h2));
        }
    }
    return result;
}

} // namespace pystring

#endif // PYSTRING_ALGO_HPP
