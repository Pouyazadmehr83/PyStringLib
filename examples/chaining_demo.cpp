#include "pystring/pystring.hpp"
#include <iostream>

int main() {
    using namespace pystring;

    std::cout << "===============================================\n";
    std::cout << "     PyStringLib: Fluent Method Chaining       \n";
    std::cout << "===============================================\n\n";

    // Scenario: Data pipeline cleaning dirty user input
    String raw_input = "  \t\r\n  pOUYA_ZADMEHR@GmAiL.COM  \n  ";
    std::cout << "Raw input:   [" << raw_input << "]\n";

    // Fluent pipeline:
    String normalized_email = raw_input
        .strip()
        .lower();

    std::cout << "Clean email: [" << normalized_email << "]\n";

    // Domain extraction and customization
    auto [name, at, domain] = normalized_email.partition("@");
    String formatted_name = name
        .replace("_", " ")
        .title();

    std::cout << "User Name:   [" << formatted_name << "]\n";
    std::cout << "Email host:  [" << domain << "]\n\n";

    // Another example: Transforming slugs
    String title = "  10 Ways Pythonic C++ Makes Life Better!!  ";
    String slug = title
        .strip()
        .lower()
        .replace(" ", "-")
        .removesuffix("!!")
        + ".html";

    std::cout << "Original: " << title << "\n";
    std::cout << "Slug URL: " << slug << "\n";

    return 0;
}
