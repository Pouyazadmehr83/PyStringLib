#ifndef PYSTRING_SEARCH_HPP
#define PYSTRING_SEARCH_HPP

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

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

#endif // PYSTRING_SEARCH_HPP
