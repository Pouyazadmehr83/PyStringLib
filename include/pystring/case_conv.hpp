#ifndef PYSTRING_CASE_CONV_HPP
#define PYSTRING_CASE_CONV_HPP

#include <cctype>
#include <string>
#include <string_view>
#include <vector>

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

#endif // PYSTRING_CASE_CONV_HPP
