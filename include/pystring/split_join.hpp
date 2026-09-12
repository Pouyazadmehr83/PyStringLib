#ifndef PYSTRING_SPLIT_JOIN_HPP
#define PYSTRING_SPLIT_JOIN_HPP

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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

#endif // PYSTRING_SPLIT_JOIN_HPP
