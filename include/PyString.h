#ifndef PYSTRING_H
#define PYSTRING_H

#include "pystring/pystring.hpp"
#include <string>

/**
 * @class PyString
 * @brief Legacy wrapper class for backward compatibility with earlier PyStringLib versions.
 * New code should prefer pystring::String or free functions in namespace pystring.
 */
class PyString {
public:
    std::string removeVowels(const std::string& str) {
        return pystring::removeVowels(str);
    }

    std::string slice(const std::string& str, int start, int end, int step) {
        return pystring::slice(str, start, end, step);
    }

    std::string revers(const std::string& str) {
        return pystring::revers(str);
    }

    int find(const std::string& haystack, const std::string& needle) {
        return static_cast<int>(pystring::find(haystack, needle));
    }

    int count(const std::string& str, const std::string& substr) {
        return static_cast<int>(pystring::count(str, substr));
    }

    std::string replace(const std::string& str, const std::string& tar, const std::string& rep) {
        return pystring::replace(str, tar, rep);
    }
};

#endif // PYSTRING_H