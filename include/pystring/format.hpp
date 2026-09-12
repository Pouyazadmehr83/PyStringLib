#ifndef PYSTRING_FORMAT_HPP
#define PYSTRING_FORMAT_HPP

#include <initializer_list>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

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

#endif // PYSTRING_FORMAT_HPP
