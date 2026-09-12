#include "pystring/pystring.hpp"
#include <iostream>

int main() {
    using namespace pystring;

    std::cout << "===============================================\n";
    std::cout << "       PyStringLib: Basic Usage Demo           \n";
    std::cout << "===============================================\n\n";

    // 1. Creation and conversion
    String greeting = "  Hello, Pythonic C++ World!  ";
    std::cout << "Original string: [" << greeting << "]\n";

    // 2. Stripping and case conversion
    String cleaned = greeting.strip();
    std::cout << "Strip:           [" << cleaned << "]\n";
    std::cout << "Lower:           [" << cleaned.lower() << "]\n";
    std::cout << "Upper:           [" << cleaned.upper() << "]\n";
    std::cout << "Title:           [" << cleaned.title() << "]\n";

    // 3. Predicates
    std::cout << "\nPredicates:\n";
    std::cout << "  '12345'.isdigit()        -> " << (String("12345").isdigit() ? "true" : "false") << "\n";
    std::cout << "  'Python'.isalpha()       -> " << (String("Python").isalpha() ? "true" : "false") << "\n";
    std::cout << "  'var_name_1'.isidentifier() -> " << (String("var_name_1").isidentifier() ? "true" : "false") << "\n";

    // 4. Searching and counting
    String quote = "The quick brown fox jumps over the lazy dog";
    std::cout << "\nSearch in: \"" << quote << "\"\n";
    std::cout << "  find('fox'):             " << quote.find("fox") << "\n";
    std::cout << "  count('o'):              " << quote.count("o") << "\n";
    std::cout << "  startswith('The'):       " << (quote.startswith("The") ? "true" : "false") << "\n";
    std::cout << "  endswith('cat'):         " << (quote.endswith("cat") ? "true" : "false") << "\n";

    // 5. Partitioning
    String email = "pouya.zadmehr@example.com";
    auto [user, at, domain] = email.partition("@");
    std::cout << "\nPartition '" << email << "':\n";
    std::cout << "  User:   " << user << "\n";
    std::cout << "  Domain: " << domain << "\n";

    // 6. Splitting & Joining
    String csv = "apple,banana,orange,grape";
    auto fruits = csv.split(",");
    std::cout << "\nSplit CSV: " << fruits.size() << " items found.\n";
    for (size_t i = 0; i < fruits.size(); ++i) {
        std::cout << "  [" << i << "] " << fruits[i] << "\n";
    }

    String joined = String(" | ").join(fruits);
    std::cout << "Rejoined: " << joined << "\n";

    // 7. Operators: Repetition and Negative Indexing
    String star = "*";
    std::cout << "\nPython operators:\n";
    std::cout << "  '*' * 30: " << (star * 30) << "\n";
    std::cout << "  greeting[-3]: '" << greeting[-3] << "'\n";

    // 8. Pythonic String Formatting
    String msg = format("User {0} achieved a score of {1} in {2}!", "Pouya", 98, "C++");
    std::cout << "\nFormat:\n  " << msg << "\n";

    return 0;
}
