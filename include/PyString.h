#ifndef PYSTRING_H
#define PYSTRING_H

#include <string>

class PyString {
public:
    std::string removeVowels(const std::string& str);
    std::string slice(const std::string& str, int start, int end,int step);
    std::string revers(const std::string& str);
    int find(const std::string& haystack, const std::string& needle);
    int count(const std::string& str, const std::string& substr);
    std::string replace(const std::string& string,const std::string& tar,std::string& rep);
};

#endif