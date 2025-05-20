#include "PyString.h"
#include <iostream>

using namespace std;

string PyString::removeVowels(const string& str) {
    string result;
    for (char c : str) {
        if (string("aeiouAEIOU").find(c) == string::npos)
            result += c;
    }
    return result;
}

string PyString::slice(const string& str, int start, int end, int step) {
    string result;
    int len = str.length();

    
    if (start < 0) start += len;
    if (end < 0) end += len;

    
    if (start < 0) start = 0;
    if (end < 0) end = 0;
    if (start >= len) start = len - 1;
    if (end >= len) end = len - 1;

    if (step == 0) {
        cout << "Step cannot be 0!" << endl;
        return "";
    }

    
    if (step > 0) {
        for (int i = start; i < end && i < len; i += step) {
            result += str[i];
        }
    }
    
    else {
        for (int i = start; i > end && i >= 0; i += step) {
            result += str[i];
        }
    }

    return result;
}

string PyString::revers(const std::string& str){
    string result;
    for (int i = str.length()-1;i >= 0; --i)
    {
        result += str[i];
    }
    return result;  
}
int PyString::find(const std::string& haystack, const std::string& needle) {
    int hlen = haystack.length();
    int nlen = needle.length();

    for (int i = 0; i <= hlen - nlen; ++i) {
        if (haystack.substr(i, nlen) == needle) {
            return i;
        }
    }

    std::cout << "not found!" << std::endl;
    return -1;
}
int PyString::count(const std::string& str, const std::string& substr) {
    int count = 0;
    int n = str.length();
    int m = substr.length();

    if (m == 0 || m > n) return 0;

    for (int i = 0; i <= n - m; ++i) {
        if (str.substr(i, m) == substr) {
            count++;
        }
    }
    return count;
}
std::string PyString::replace(const std::string& str, const std::string& tar, std::string& rep) {
    if (tar.empty()) {
        return str;
    } else if (str.empty() || rep.empty()) {
        std::cout << "missing value!!" << std::endl;
        return str;  
    }

 
    std::string result;
    //...
    return result;
}

