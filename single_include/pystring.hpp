/**
 * PyStringLib - Single-Header Amalgamated Edition
 * Pythonic String Library for Modern C++ (C++17 / C++20 / C++23)
 * 
 * GitHub: https://github.com/Pouyazadmehr83/PyStringLib
 * License: MIT
 */

#ifndef PYSTRING_SINGLE_HEADER_HPP
#define PYSTRING_SINGLE_HEADER_HPP

// Standard Library Dependencies
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// --- Begin: slice.hpp ---


namespace pystring {

/**
 * @brief Python-style slice object representing [start:stop:step].
 */
struct Slice {
    std::optional<ptrdiff_t> start;
    std::optional<ptrdiff_t> stop;
    std::optional<ptrdiff_t> step;

    constexpr Slice() : start(std::nullopt), stop(std::nullopt), step(std::nullopt) {}
    
    constexpr Slice(std::optional<ptrdiff_t> stop_val)
        : start(std::nullopt), stop(stop_val), step(std::nullopt) {}
        
    constexpr Slice(std::optional<ptrdiff_t> start_val, std::optional<ptrdiff_t> stop_val)
        : start(start_val), stop(stop_val), step(std::nullopt) {}
        
    constexpr Slice(std::optional<ptrdiff_t> start_val, 
                    std::optional<ptrdiff_t> stop_val, 
                    std::optional<ptrdiff_t> step_val)
        : start(start_val), stop(stop_val), step(step_val) {}

    struct ComputedIndices {
        ptrdiff_t start{0};
        ptrdiff_t stop{0};
        ptrdiff_t step{1};
        size_t length{0};
    };

    /**
     * @brief Computes concrete iteration indices according to CPython's PySlice_GetIndicesEx.
     */
    constexpr ComputedIndices compute(size_t len) const {
        ptrdiff_t st = step.value_or(1);
        if (st == 0) {
            throw std::invalid_argument("slice step cannot be zero");
        }

        ptrdiff_t s = 0;
        ptrdiff_t e = 0;
        ptrdiff_t length = static_cast<ptrdiff_t>(len);

        if (st > 0) {
            s = start.value_or(0);
            e = stop.value_or(length);

            if (s < 0) s += length;
            if (s < 0) s = 0;
            if (s > length) s = length;

            if (e < 0) e += length;
            if (e < 0) e = 0;
            if (e > length) e = length;
        } else {
            s = start.value_or(length - 1);

            if (s < 0) s += length;
            if (s < -1) s = -1;
            if (s >= length) s = length - 1;

            if (stop.has_value()) {
                e = *stop;
                if (e < 0) e += length;
                if (e < -1) e = -1;
                if (e >= length) e = length - 1;
            } else {
                e = -1;
            }
        }

        size_t slice_len = 0;
        if (st > 0) {
            if (s < e) {
                slice_len = static_cast<size_t>((e - s - 1) / st + 1);
            }
        } else {
            if (s > e) {
                slice_len = static_cast<size_t>((s - e - 1) / (-st) + 1);
            }
        }

        return {s, e, st, slice_len};
    }
};

/**
 * @brief Helper factory function to create a Slice(stop).
 */
inline constexpr Slice slice(std::optional<ptrdiff_t> stop = std::nullopt) {
    return Slice(stop);
}

/**
 * @brief Helper factory function to create a Slice(start, stop, step).
 */
inline constexpr Slice slice(std::optional<ptrdiff_t> start, 
                             std::optional<ptrdiff_t> stop, 
                             std::optional<ptrdiff_t> step = std::nullopt) {
    return Slice(start, stop, step);
}

} // namespace pystring


// --- End: slice.hpp ---

// --- Begin: predicates.hpp ---


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


// --- End: predicates.hpp ---

// --- Begin: search.hpp ---


namespace pystring {

namespace detail {

inline std::pair<size_t, size_t> normalize_slice_bounds(size_t len, ptrdiff_t start, std::optional<ptrdiff_t> end) {
    ptrdiff_t s = start;
    ptrdiff_t e = end.value_or(static_cast<ptrdiff_t>(len));
    ptrdiff_t length = static_cast<ptrdiff_t>(len);

    if (s < 0) s += length;
    if (s < 0) s = 0;
    if (s > length) s = length;

    if (e < 0) e += length;
    if (e < 0) e = 0;
    if (e > length) e = length;

    if (s > e) s = e;
    return {static_cast<size_t>(s), static_cast<size_t>(e)};
}

} // namespace detail

/**
 * @brief Returns the lowest index where sub is found in s[start:end], or -1 if not found.
 */
inline ptrdiff_t find(std::string_view s, std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    auto [s_idx, e_idx] = detail::normalize_slice_bounds(s.size(), start, end);
    if (s_idx >= e_idx && !sub.empty()) {
        return -1;
    }
    if (sub.empty()) {
        return static_cast<ptrdiff_t>(s_idx);
    }
    std::string_view sub_view = s.substr(s_idx, e_idx - s_idx);
    size_t pos = sub_view.find(sub);
    if (pos == std::string_view::npos) {
        return -1;
    }
    return static_cast<ptrdiff_t>(s_idx + pos);
}

inline ptrdiff_t find(std::string_view s, char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    char buf[1] = {ch};
    return find(s, std::string_view(buf, 1), start, end);
}

/**
 * @brief Returns the highest index where sub is found in s[start:end], or -1 if not found.
 */
inline ptrdiff_t rfind(std::string_view s, std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    auto [s_idx, e_idx] = detail::normalize_slice_bounds(s.size(), start, end);
    if (s_idx >= e_idx && !sub.empty()) {
        return -1;
    }
    if (sub.empty()) {
        return static_cast<ptrdiff_t>(e_idx);
    }
    std::string_view sub_view = s.substr(s_idx, e_idx - s_idx);
    size_t pos = sub_view.rfind(sub);
    if (pos == std::string_view::npos) {
        return -1;
    }
    return static_cast<ptrdiff_t>(s_idx + pos);
}

inline ptrdiff_t rfind(std::string_view s, char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    char buf[1] = {ch};
    return rfind(s, std::string_view(buf, 1), start, end);
}

/**
 * @brief Like find, but throws std::out_of_range if sub is not found.
 */
inline size_t index(std::string_view s, std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) {
    ptrdiff_t res = find(s, sub, start, end);
    if (res < 0) {
        throw std::out_of_range("substring not found in pystring::index");
    }
    return static_cast<size_t>(res);
}

inline size_t index(std::string_view s, char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) {
    char buf[1] = {ch};
    return index(s, std::string_view(buf, 1), start, end);
}

/**
 * @brief Like rfind, but throws std::out_of_range if sub is not found.
 */
inline size_t rindex(std::string_view s, std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) {
    ptrdiff_t res = rfind(s, sub, start, end);
    if (res < 0) {
        throw std::out_of_range("substring not found in pystring::rindex");
    }
    return static_cast<size_t>(res);
}

inline size_t rindex(std::string_view s, char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) {
    char buf[1] = {ch};
    return rindex(s, std::string_view(buf, 1), start, end);
}

/**
 * @brief Returns the number of non-overlapping occurrences of sub in s[start:end].
 */
inline size_t count(std::string_view s, std::string_view sub, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    auto [s_idx, e_idx] = detail::normalize_slice_bounds(s.size(), start, end);
    if (s_idx > e_idx) return 0;
    std::string_view window = s.substr(s_idx, e_idx - s_idx);

    if (sub.empty()) {
        return window.size() + 1;
    }

    size_t cnt = 0;
    size_t pos = 0;
    while ((pos = window.find(sub, pos)) != std::string_view::npos) {
        ++cnt;
        pos += sub.size();
    }
    return cnt;
}

inline size_t count(std::string_view s, char ch, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    char buf[1] = {ch};
    return count(s, std::string_view(buf, 1), start, end);
}

/**
 * @brief Checks if string starts with prefix in slice s[start:end].
 */
inline bool startswith(std::string_view s, std::string_view prefix, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    auto [s_idx, e_idx] = detail::normalize_slice_bounds(s.size(), start, end);
    std::string_view sub = s.substr(s_idx, e_idx - s_idx);
    if (prefix.size() > sub.size()) return false;
    return sub.substr(0, prefix.size()) == prefix;
}

inline bool startswith(std::string_view s, std::initializer_list<std::string_view> prefixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    for (auto prefix : prefixes) {
        if (startswith(s, prefix, start, end)) return true;
    }
    return false;
}

inline bool startswith(std::string_view s, const std::vector<std::string>& prefixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    for (const auto& prefix : prefixes) {
        if (startswith(s, prefix, start, end)) return true;
    }
    return false;
}

/**
 * @brief Checks if string ends with suffix in slice s[start:end].
 */
inline bool endswith(std::string_view s, std::string_view suffix, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    auto [s_idx, e_idx] = detail::normalize_slice_bounds(s.size(), start, end);
    std::string_view sub = s.substr(s_idx, e_idx - s_idx);
    if (suffix.size() > sub.size()) return false;
    return sub.substr(sub.size() - suffix.size()) == suffix;
}

inline bool endswith(std::string_view s, std::initializer_list<std::string_view> suffixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    for (auto suffix : suffixes) {
        if (endswith(s, suffix, start, end)) return true;
    }
    return false;
}

inline bool endswith(std::string_view s, const std::vector<std::string>& suffixes, ptrdiff_t start = 0, std::optional<ptrdiff_t> end = std::nullopt) noexcept {
    for (const auto& suffix : suffixes) {
        if (endswith(s, suffix, start, end)) return true;
    }
    return false;
}

/**
 * @brief Partition string at first occurrence of sep.
 * Returns {head, sep, tail}. If sep is not found, returns {s, "", ""}.
 */
inline std::tuple<std::string_view, std::string_view, std::string_view> partition_view(std::string_view s, std::string_view sep) {
    if (sep.empty()) {
        throw std::invalid_argument("empty separator in partition");
    }
    size_t pos = s.find(sep);
    if (pos == std::string_view::npos) {
        return {s, std::string_view{}, std::string_view{}};
    }
    return {
        s.substr(0, pos),
        s.substr(pos, sep.size()),
        s.substr(pos + sep.size())
    };
}

inline std::tuple<std::string, std::string, std::string> partition(std::string_view s, std::string_view sep) {
    auto [head, match, tail] = partition_view(s, sep);
    return {std::string(head), std::string(match), std::string(tail)};
}

/**
 * @brief Partition string at last occurrence of sep.
 * Returns {head, sep, tail}. If sep is not found, returns {"", "", s}.
 */
inline std::tuple<std::string_view, std::string_view, std::string_view> rpartition_view(std::string_view s, std::string_view sep) {
    if (sep.empty()) {
        throw std::invalid_argument("empty separator in rpartition");
    }
    size_t pos = s.rfind(sep);
    if (pos == std::string_view::npos) {
        return {std::string_view{}, std::string_view{}, s};
    }
    return {
        s.substr(0, pos),
        s.substr(pos, sep.size()),
        s.substr(pos + sep.size())
    };
}

inline std::tuple<std::string, std::string, std::string> rpartition(std::string_view s, std::string_view sep) {
    auto [head, match, tail] = rpartition_view(s, sep);
    return {std::string(head), std::string(match), std::string(tail)};
}

} // namespace pystring


// --- End: search.hpp ---

// --- Begin: split_join.hpp ---


namespace pystring {

namespace detail {

inline bool is_space(char c) noexcept {
    return std::isspace(static_cast<unsigned char>(c));
}

template <typename T>
inline std::string to_string_item(const T& item) {
    if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
        return item;
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::string_view>) {
        return std::string(item);
    } else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
        return std::string(item);
    } else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
        return std::to_string(item);
    } else {
        std::ostringstream oss;
        oss << item;
        return oss.str();
    }
}

} // namespace detail

/**
 * @brief Zero-copy split returning string_views.
 * If sep is empty, splits by contiguous whitespace and ignores leading/trailing spaces (like Python's s.split()).
 */
inline std::vector<std::string_view> split_view(std::string_view s, std::string_view sep = "", ptrdiff_t maxsplit = -1) {
    std::vector<std::string_view> result;
    if (maxsplit == 0) {
        result.push_back(s);
        return result;
    }

    if (sep.empty()) {
        // Python whitespace splitting mode
        size_t n = s.size();
        size_t i = 0;
        ptrdiff_t splits_done = 0;

        while (i < n) {
            // Skip whitespace
            while (i < n && detail::is_space(s[i])) {
                ++i;
            }
            if (i >= n) break;

            if (maxsplit >= 0 && splits_done >= maxsplit) {
                // Last token takes the rest, but stripped of leading whitespace
                // Python keeps trailing whitespace in the last element if maxsplit reached
                result.push_back(s.substr(i));
                break;
            }

            size_t start = i;
            while (i < n && !detail::is_space(s[i])) {
                ++i;
            }
            result.push_back(s.substr(start, i - start));
            ++splits_done;
        }
        return result;
    }

    // Exact separator mode
    size_t start = 0;
    ptrdiff_t splits_done = 0;
    while (maxsplit < 0 || splits_done < maxsplit) {
        size_t pos = s.find(sep, start);
        if (pos == std::string_view::npos) {
            break;
        }
        result.push_back(s.substr(start, pos - start));
        start = pos + sep.size();
        ++splits_done;
    }
    result.push_back(s.substr(start));
    return result;
}

/**
 * @brief Splits s by sep, returning std::vector<std::string>.
 */
inline std::vector<std::string> split(std::string_view s, std::string_view sep = "", ptrdiff_t maxsplit = -1) {
    auto views = split_view(s, sep, maxsplit);
    std::vector<std::string> result;
    result.reserve(views.size());
    for (auto v : views) {
        result.emplace_back(v);
    }
    return result;
}

/**
 * @brief Zero-copy rsplit returning string_views, splitting from the right.
 */
inline std::vector<std::string_view> rsplit_view(std::string_view s, std::string_view sep = "", ptrdiff_t maxsplit = -1) {
    if (maxsplit < 0) {
        return split_view(s, sep, -1);
    }
    if (maxsplit == 0) {
        return {s};
    }

    std::vector<std::string_view> result;
    if (sep.empty()) {
        // Python whitespace rsplit
        ptrdiff_t i = static_cast<ptrdiff_t>(s.size()) - 1;
        ptrdiff_t splits_done = 0;

        while (i >= 0) {
            while (i >= 0 && detail::is_space(s[static_cast<size_t>(i)])) {
                --i;
            }
            if (i < 0) break;

            if (splits_done >= maxsplit) {
                // First element takes the remaining prefix up to i+1
                // Python strips trailing whitespace before i+1
                result.push_back(s.substr(0, static_cast<size_t>(i + 1)));
                break;
            }

            ptrdiff_t end_idx = i;
            while (i >= 0 && !detail::is_space(s[static_cast<size_t>(i)])) {
                --i;
            }
            result.push_back(s.substr(static_cast<size_t>(i + 1), static_cast<size_t>(end_idx - i)));
            ++splits_done;
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    // Explicit separator rsplit
    ptrdiff_t end_idx = static_cast<ptrdiff_t>(s.size());
    ptrdiff_t splits_done = 0;

    while (splits_done < maxsplit) {
        if (end_idx < static_cast<ptrdiff_t>(sep.size())) {
            break;
        }
        std::string_view sub = s.substr(0, static_cast<size_t>(end_idx));
        size_t pos = sub.rfind(sep);
        if (pos == std::string_view::npos) {
            break;
        }
        result.push_back(s.substr(pos + sep.size(), static_cast<size_t>(end_idx) - (pos + sep.size())));
        end_idx = static_cast<ptrdiff_t>(pos);
        ++splits_done;
    }
    result.push_back(s.substr(0, static_cast<size_t>(end_idx)));
    std::reverse(result.begin(), result.end());
    return result;
}

/**
 * @brief Splits s by sep from right, returning std::vector<std::string>.
 */
inline std::vector<std::string> rsplit(std::string_view s, std::string_view sep = "", ptrdiff_t maxsplit = -1) {
    auto views = rsplit_view(s, sep, maxsplit);
    std::vector<std::string> result;
    result.reserve(views.size());
    for (auto v : views) {
        result.emplace_back(v);
    }
    return result;
}

/**
 * @brief Zero-copy splitlines returning string_views.
 */
inline std::vector<std::string_view> splitlines_view(std::string_view s, bool keepends = false) {
    std::vector<std::string_view> result;
    size_t i = 0;
    size_t n = s.size();

    while (i < n) {
        size_t line_start = i;
        while (i < n && s[i] != '\n' && s[i] != '\r' && s[i] != '\v' && s[i] != '\f' &&
               s[i] != '\x1c' && s[i] != '\x1d' && s[i] != '\x1e') {
            ++i;
        }

        size_t line_end = i;
        if (i < n) {
            if (s[i] == '\r' && i + 1 < n && s[i + 1] == '\n') {
                i += 2;
            } else {
                i += 1;
            }
            if (keepends) {
                result.push_back(s.substr(line_start, i - line_start));
            } else {
                result.push_back(s.substr(line_start, line_end - line_start));
            }
        } else {
            result.push_back(s.substr(line_start, line_end - line_start));
        }
    }
    return result;
}

/**
 * @brief Splits s at line breaks, returning std::vector<std::string>.
 */
inline std::vector<std::string> splitlines(std::string_view s, bool keepends = false) {
    auto views = splitlines_view(s, keepends);
    std::vector<std::string> result;
    result.reserve(views.size());
    for (auto v : views) {
        result.emplace_back(v);
    }
    return result;
}

/**
 * @brief Joins an iterable container of elements using sep.
 */
template <typename Container>
inline std::string join(std::string_view sep, const Container& items) {
    auto it = std::begin(items);
    auto end_it = std::end(items);
    if (it == end_it) {
        return "";
    }

    std::string result = detail::to_string_item(*it);
    ++it;
    for (; it != end_it; ++it) {
        result.append(sep);
        result.append(detail::to_string_item(*it));
    }
    return result;
}

/**
 * @brief Joins elements from an initializer_list.
 */
template <typename T>
inline std::string join(std::string_view sep, std::initializer_list<T> items) {
    return join(sep, std::vector<T>(items));
}

/**
 * @brief Joins an iterator range [first, last).
 */
template <typename InputIt>
inline std::string join(std::string_view sep, InputIt first, InputIt last) {
    if (first == last) return "";
    std::string result = detail::to_string_item(*first);
    ++first;
    for (; first != last; ++first) {
        result.append(sep);
        result.append(detail::to_string_item(*first));
    }
    return result;
}

} // namespace pystring


// --- End: split_join.hpp ---

// --- Begin: ops.hpp ---


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


// --- End: ops.hpp ---

// --- Begin: format.hpp ---


namespace pystring {

namespace detail {

template <typename T>
inline std::string to_format_string(const T& val) {
    if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
        return val;
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::string_view>) {
        return std::string(val);
    } else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
        return std::string(val);
    } else if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
        return val ? "True" : "False";
    } else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
        return std::to_string(val);
    } else {
        std::ostringstream oss;
        oss << val;
        return oss.str();
    }
}

inline std::string vformat_args(std::string_view fmt, const std::vector<std::string>& args) {
    std::string result;
    result.reserve(fmt.size() + args.size() * 16);
    size_t auto_idx = 0;
    size_t i = 0;
    size_t n = fmt.size();

    while (i < n) {
        if (fmt[i] == '{') {
            if (i + 1 < n && fmt[i + 1] == '{') {
                result.push_back('{');
                i += 2;
                continue;
            }

            size_t close = fmt.find('}', i + 1);
            if (close == std::string_view::npos) {
                result.push_back(fmt[i]);
                ++i;
                continue;
            }

            std::string_view placeholder = fmt.substr(i + 1, close - i - 1);
            if (placeholder.empty()) {
                // Auto indexed {}
                if (auto_idx < args.size()) {
                    result.append(args[auto_idx++]);
                }
            } else {
                // Check if integer index {0}, {1}
                bool is_num = true;
                for (char c : placeholder) {
                    if (!std::isdigit(static_cast<unsigned char>(c))) {
                        is_num = false;
                        break;
                    }
                }
                if (is_num) {
                    size_t arg_idx = static_cast<size_t>(std::stoul(std::string(placeholder)));
                    if (arg_idx < args.size()) {
                        result.append(args[arg_idx]);
                    }
                }
            }
            i = close + 1;
        } else if (fmt[i] == '}') {
            if (i + 1 < n && fmt[i + 1] == '}') {
                result.push_back('}');
                i += 2;
                continue;
            }
            result.push_back(fmt[i]);
            ++i;
        } else {
            result.push_back(fmt[i]);
            ++i;
        }
    }
    return result;
}

} // namespace detail

/**
 * @brief Python-like format using variadic arguments: format("Hello {}!", name).
 * Supports positional {} and numbered {0}, {1} placeholders.
 */
template <typename... Args>
inline std::string format(std::string_view fmt, const Args&... args) {
    if constexpr (sizeof...(Args) == 0) {
        return std::string(fmt);
    } else {
        std::vector<std::string> str_args = { detail::to_format_string(args)... };
        return detail::vformat_args(fmt, str_args);
    }
}

/**
 * @brief Python-like keyword format: format("{name} is {age}", {{"name", "Alice"}, {"age", "25"}}).
 */
inline std::string format_map(std::string_view fmt, const std::unordered_map<std::string, std::string>& mapping) {
    std::string result;
    result.reserve(fmt.size() + 32);
    size_t i = 0;
    size_t n = fmt.size();

    while (i < n) {
        if (fmt[i] == '{') {
            if (i + 1 < n && fmt[i + 1] == '{') {
                result.push_back('{');
                i += 2;
                continue;
            }
            size_t close = fmt.find('}', i + 1);
            if (close == std::string_view::npos) {
                result.push_back(fmt[i]);
                ++i;
                continue;
            }
            std::string key(fmt.substr(i + 1, close - i - 1));
            auto it = mapping.find(key);
            if (it != mapping.end()) {
                result.append(it->second);
            } else {
                result.push_back('{');
                result.append(key);
                result.push_back('}');
            }
            i = close + 1;
        } else if (fmt[i] == '}') {
            if (i + 1 < n && fmt[i + 1] == '}') {
                result.push_back('}');
                i += 2;
                continue;
            }
            result.push_back(fmt[i]);
            ++i;
        } else {
            result.push_back(fmt[i]);
            ++i;
        }
    }
    return result;
}

inline std::string format_map(std::string_view fmt, std::initializer_list<std::pair<std::string, std::string>> mapping) {
    std::unordered_map<std::string, std::string> map_obj;
    for (const auto& kv : mapping) {
        map_obj[kv.first] = kv.second;
    }
    return format_map(fmt, map_obj);
}

} // namespace pystring


// --- End: format.hpp ---

// --- Begin: utf8.hpp ---


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


// --- End: utf8.hpp ---

// --- Begin: case_conv.hpp ---


namespace pystring {

namespace detail {

/**
 * @brief Splits an identifier string into logical words based on case boundaries, dashes, underscores, and spaces.
 */
inline std::vector<std::string> extract_words(std::string_view s) {
    std::vector<std::string> words;
    std::string current;

    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        if (c == '_' || c == '-' || std::isspace(c)) {
            if (!current.empty()) {
                words.push_back(std::move(current));
                current.clear();
            }
        } else if (std::isupper(c)) {
            // Check if camelCase boundary: prev char is lowercase OR next char is lowercase
            bool prev_is_lower = (i > 0 && std::islower(static_cast<unsigned char>(s[i - 1])));
            bool next_is_lower = (i + 1 < s.size() && std::islower(static_cast<unsigned char>(s[i + 1])));

            if (prev_is_lower || (!current.empty() && next_is_lower && current.size() > 1)) {
                if (!current.empty()) {
                    words.push_back(std::move(current));
                    current.clear();
                }
            }
            current.push_back(static_cast<char>(std::tolower(c)));
        } else {
            current.push_back(static_cast<char>(std::tolower(c)));
        }
    }

    if (!current.empty()) {
        words.push_back(std::move(current));
    }
    return words;
}

} // namespace detail

/**
 * @brief Converts string to snake_case (e.g. "userFirstName" -> "user_first_name").
 */
inline std::string to_snake_case(std::string_view s) {
    auto words = detail::extract_words(s);
    if (words.empty()) return "";

    std::string result = words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        result.push_back('_');
        result.append(words[i]);
    }
    return result;
}

/**
 * @brief Converts string to kebab-case (e.g. "userFirstName" -> "user-first-name").
 */
inline std::string to_kebab_case(std::string_view s) {
    auto words = detail::extract_words(s);
    if (words.empty()) return "";

    std::string result = words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        result.push_back('-');
        result.append(words[i]);
    }
    return result;
}

/**
 * @brief Converts string to camelCase (e.g. "user_first_name" -> "userFirstName").
 */
inline std::string to_camel_case(std::string_view s) {
    auto words = detail::extract_words(s);
    if (words.empty()) return "";

    std::string result = words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        std::string w = words[i];
        if (!w.empty()) {
            w[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(w[0])));
        }
        result.append(w);
    }
    return result;
}

/**
 * @brief Converts string to PascalCase (e.g. "user_first_name" -> "UserFirstName").
 */
inline std::string to_pascal_case(std::string_view s) {
    auto words = detail::extract_words(s);
    if (words.empty()) return "";

    std::string result;
    for (size_t i = 0; i < words.size(); ++i) {
        std::string w = words[i];
        if (!w.empty()) {
            w[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(w[0])));
        }
        result.append(w);
    }
    return result;
}

} // namespace pystring


// --- End: case_conv.hpp ---

// --- Begin: regex_ops.hpp ---


namespace pystring {

/**
 * @brief Checks if entire string matches regex pattern (like Python re.fullmatch).
 */
inline bool matches(std::string_view s, std::string_view pattern) {
    try {
        std::regex re(pattern.data(), pattern.size());
        return std::regex_match(s.begin(), s.end(), re);
    } catch (const std::regex_error&) {
        return false;
    }
}

/**
 * @brief Checks if regex pattern is found anywhere in string (like Python re.search).
 */
inline bool search_regex(std::string_view s, std::string_view pattern) {
    try {
        std::regex re(pattern.data(), pattern.size());
        return std::regex_search(s.begin(), s.end(), re);
    } catch (const std::regex_error&) {
        return false;
    }
}

/**
 * @brief Replaces occurrences matching regex pattern with replacement (like Python re.sub).
 */
inline std::string replace_regex(std::string_view s, std::string_view pattern, std::string_view replacement) {
    try {
        std::regex re(pattern.data(), pattern.size());
        std::string str(s);
        std::string rep(replacement);
        return std::regex_replace(str, re, rep);
    } catch (const std::regex_error&) {
        return std::string(s);
    }
}

/**
 * @brief Splits string by regex pattern delimiters (like Python re.split).
 */
inline std::vector<std::string> split_regex(std::string_view s, std::string_view pattern) {
    std::vector<std::string> result;
    try {
        std::regex re(pattern.data(), pattern.size());
        std::string str(s);
        std::sregex_token_iterator iter(str.begin(), str.end(), re, -1);
        std::sregex_token_iterator end;
        for (; iter != end; ++iter) {
            result.push_back(*iter);
        }
    } catch (const std::regex_error&) {
        result.emplace_back(s);
    }
    return result;
}

/**
 * @brief Finds all non-overlapping regex matches in string (like Python re.findall).
 */
inline std::vector<std::string> findall(std::string_view s, std::string_view pattern) {
    std::vector<std::string> result;
    try {
        std::regex re(pattern.data(), pattern.size());
        std::string str(s);
        auto words_begin = std::sregex_iterator(str.begin(), str.end(), re);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            result.push_back(i->str());
        }
    } catch (const std::regex_error&) {
        // Return empty vector on invalid regex
    }
    return result;
}

} // namespace pystring


// --- End: regex_ops.hpp ---

// --- Begin: algo.hpp ---


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


// --- End: algo.hpp ---

// --- Begin: core.hpp ---



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


// --- End: core.hpp ---

// --- Begin: pystring.hpp ---

/**
 * @file pystring.hpp
 * @brief Main umbrella header for PyStringLib.
 * 
 * PyStringLib is a modern C++ library bringing Python's powerful and elegant
 * string manipulation capabilities to C++17/20/23 with zero overhead,
 * method chaining, zero-copy string views, UTF-8 awareness, and Pythonic syntax.
 * 
 * Developed by Pouya Zadmehr (Pouyazadmehr83).
 */


namespace pystring {
    // Pythonic alias: pystring::str
    using str = String;
}


// --- End: pystring.hpp ---

#endif // PYSTRING_SINGLE_HEADER_HPP
