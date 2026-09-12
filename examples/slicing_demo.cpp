#include "pystring/pystring.hpp"
#include <iostream>
#include <iomanip>

int main() {
    using namespace pystring;

    String s = "0123456789";
    std::cout << "===============================================\n";
    std::cout << "       PyStringLib: Slicing Showcase           \n";
    std::cout << "===============================================\n\n";

    std::cout << "Target String: " << s << "\n\n";

    // Visual diagram
    std::cout << "Positive Index:  0  1  2  3  4  5  6  7  8  9\n";
    std::cout << "Characters:      0  1  2  3  4  5  6  7  8  9\n";
    std::cout << "Negative Index: -10 -9 -8 -7 -6 -5 -4 -3 -2 -1\n\n";

    auto show_slice = [&](const char* expr, const String& result) {
        std::cout << std::left << std::setw(25) << expr 
                  << " ->  \"" << result << "\"\n";
    };

    std::cout << "-- Standard Substrings --\n";
    show_slice("s(2, 6)", s(2, 6));                     // "2345"
    show_slice("s[slice(0, 5)]", s[slice(0, 5)]);       // "01234"
    show_slice("s(5, std::nullopt)", s(5, std::nullopt)); // "56789"
    show_slice("s(std::nullopt, 4)", s(std::nullopt, 4)); // "0123"

    std::cout << "\n-- Stepping (stride) --\n";
    show_slice("s(0, 10, 2)", s(0, 10, 2));             // "02468"
    show_slice("s(1, 10, 2)", s(1, 10, 2));             // "13579"
    show_slice("s(std::nullopt, std::nullopt, 3)", s(std::nullopt, std::nullopt, 3)); // "0369"

    std::cout << "\n-- Negative Indexing --\n";
    show_slice("s(-4, -1)", s(-4, -1));                 // "678"
    show_slice("s(-3, std::nullopt)", s(-3, std::nullopt)); // "789"
    show_slice("s(std::nullopt, -2)", s(std::nullopt, -2)); // "01234567"

    std::cout << "\n-- Reverse Slicing (Step < 0) --\n";
    show_slice("s(null, null, -1)", s(std::nullopt, std::nullopt, -1)); // "9876543210"
    show_slice("s(8, 2, -2)", s(8, 2, -2));             // "864"
    show_slice("s(-1, -6, -1)", s(-1, -6, -1));         // "98765"

    std::cout << "\n-- Direct Negative Subscript --\n";
    std::cout << "s[-1] (last character):      '" << s[-1] << "'\n";
    std::cout << "s[-2] (second-to-last):      '" << s[-2] << "'\n";
    std::cout << "s.at(-5):                    '" << s.at(-5) << "'\n";

    return 0;
}
