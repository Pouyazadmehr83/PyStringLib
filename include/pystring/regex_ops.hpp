#ifndef PYSTRING_REGEX_OPS_HPP
#define PYSTRING_REGEX_OPS_HPP

#include <regex>
#include <string>
#include <string_view>
#include <vector>

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

#endif // PYSTRING_REGEX_OPS_HPP
