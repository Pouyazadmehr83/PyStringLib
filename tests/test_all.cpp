#include "pystring/pystring.hpp"
#include "PyString.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Simple colorful test framework
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(expr, msg) \
    do { \
        if (expr) { \
            ++tests_passed; \
        } else { \
            ++tests_failed; \
            std::cerr << "\033[1;31m[FAILED]\033[0m " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while(0)

#define TEST_EQUAL(actual, expected, msg) \
    do { \
        if ((actual) == (expected)) { \
            ++tests_passed; \
        } else { \
            ++tests_failed; \
            std::cerr << "\033[1;31m[FAILED]\033[0m " << msg << " -> Expected: [" << (expected) << "] Got: [" << (actual) << "] (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while(0)

void test_slicing() {
    using namespace pystring;
    String s = "Hello, World!";
    
    // Normal slice
    TEST_EQUAL(s.slice(0, 5), "Hello", "s.slice(0, 5)");
    TEST_EQUAL(s.slice(7, 12), "World", "s.slice(7, 12)");
    
    // Default indices
    TEST_EQUAL(s.slice(std::nullopt, 5), "Hello", "s.slice(null, 5)");
    TEST_EQUAL(s.slice(7, std::nullopt), "World!", "s.slice(7, null)");
    
    // Negative indices
    TEST_EQUAL(s.slice(-6, -1), "World", "s.slice(-6, -1)");
    TEST_EQUAL(s.slice(-1, std::nullopt), "!", "s.slice(-1, null)");
    
    // Step > 1
    TEST_EQUAL(s.slice(0, 5, 2), "Hlo", "s.slice(0, 5, 2)");
    
    // Reverse stepping (negative step)
    TEST_EQUAL(s.slice(std::nullopt, std::nullopt, -1), "!dlroW ,olleH", "Full reverse");
    TEST_EQUAL(s.slice(4, std::nullopt, -1), "olleH", "s.slice(4, null, -1)");
    
    // Test slice object operator[]
    TEST_EQUAL(s[slice(0, 5)], "Hello", "s[slice(0, 5)]");
    TEST_EQUAL(s[slice(std::nullopt, std::nullopt, -1)], "!dlroW ,olleH", "s[slice(null, null, -1)]");

    // Operator()
    TEST_EQUAL(s(0, 5), "Hello", "s(0, 5)");
    TEST_EQUAL(s(7, 12), "World", "s(7, 12)");
    TEST_EQUAL(s(std::nullopt, std::nullopt, -1), "!dlroW ,olleH", "s(null, null, -1)");

    // Negative indexing: s[-1]
    TEST_EQUAL(s[-1], '!', "s[-1]");
    TEST_EQUAL(s[-2], 'd', "s[-2]");
    TEST_EQUAL(s[0], 'H', "s[0]");
    TEST_EQUAL(s.at(-1), '!', "s.at(-1)");
}

void test_predicates() {
    using namespace pystring;

    // isalpha
    TEST_ASSERT(String("Hello").isalpha(), "Hello isalpha");
    TEST_ASSERT(!String("Hello123").isalpha(), "Hello123 not isalpha");
    TEST_ASSERT(!String("").isalpha(), "Empty not isalpha");

    // isdigit / isnumeric
    TEST_ASSERT(String("123456").isdigit(), "123456 isdigit");
    TEST_ASSERT(!String("123a").isdigit(), "123a not isdigit");
    TEST_ASSERT(String("123456").isnumeric(), "123456 isnumeric");

    // isalnum
    TEST_ASSERT(String("Pouya83").isalnum(), "Pouya83 isalnum");
    TEST_ASSERT(!String("Pouya 83").isalnum(), "Pouya 83 not isalnum");

    // isspace
    TEST_ASSERT(String("   \t\n\r ").isspace(), "whitespace isspace");
    TEST_ASSERT(!String("   a  ").isspace(), "non-whitespace not isspace");
    TEST_ASSERT(!String("").isspace(), "empty not isspace");

    // islower / isupper
    TEST_ASSERT(String("hello").islower(), "hello islower");
    TEST_ASSERT(!String("Hello").islower(), "Hello not islower");
    TEST_ASSERT(String("HELLO").isupper(), "HELLO isupper");
    TEST_ASSERT(!String("Hello").isupper(), "Hello not isupper");

    // isascii
    TEST_ASSERT(String("Hello, World! 123").isascii(), "ascii isascii");
    TEST_ASSERT(String("").isascii(), "empty isascii");

    // isidentifier
    TEST_ASSERT(String("my_var_1").isidentifier(), "my_var_1 isidentifier");
    TEST_ASSERT(!String("1_var").isidentifier(), "1_var not isidentifier");
    TEST_ASSERT(!String("my-var").isidentifier(), "my-var not isidentifier");

    // istitle
    TEST_ASSERT(String("Hello World").istitle(), "Hello World istitle");
    TEST_ASSERT(!String("Hello world").istitle(), "Hello world not istitle");

    // contains
    TEST_ASSERT(String("Python in C++").contains("Python"), "contains Python");
    TEST_ASSERT(String("Python in C++").contains("C++"), "contains C++");
    TEST_ASSERT(!String("Python in C++").contains("Java"), "not contains Java");
}

void test_search() {
    using namespace pystring;
    String s = "banana";

    // find & rfind
    TEST_EQUAL(s.find("an"), 1, "find an in banana");
    TEST_EQUAL(s.rfind("an"), 3, "rfind an in banana");
    TEST_EQUAL(s.find("xyz"), -1, "find xyz not found");

    // count
    TEST_EQUAL(s.count("an"), 2, "count an in banana");
    TEST_EQUAL(s.count("a"), 3, "count a in banana");
    TEST_EQUAL(s.count(""), 7, "count empty in banana");

    // startswith & endswith
    TEST_ASSERT(s.startswith("ba"), "banana startswith ba");
    TEST_ASSERT(!s.startswith("na"), "banana not startswith na");
    TEST_ASSERT(s.endswith("na"), "banana endswith na");
    TEST_ASSERT(!s.endswith("ba"), "banana not endswith ba");
    TEST_ASSERT(s.startswith({"apple", "banana", "cherry"}), "startswith list");

    // partition & rpartition
    String email = "pouya@example.com";
    auto [user, sep, domain] = email.partition("@");
    TEST_EQUAL(user, "pouya", "partition user");
    TEST_EQUAL(sep, "@", "partition sep");
    TEST_EQUAL(domain, "example.com", "partition domain");

    String path = "/usr/local/bin/gcc";
    auto [parent, p_sep, filename] = path.rpartition("/");
    TEST_EQUAL(parent, "/usr/local/bin", "rpartition parent");
    TEST_EQUAL(p_sep, "/", "rpartition sep");
    TEST_EQUAL(filename, "gcc", "rpartition filename");
}

void test_transforms() {
    using namespace pystring;

    // Case conversions
    TEST_EQUAL(String("hello world").capitalize(), "Hello world", "capitalize");
    TEST_EQUAL(String("Hello WORLD").lower(), "hello world", "lower");
    TEST_EQUAL(String("hello world").upper(), "HELLO WORLD", "upper");
    TEST_EQUAL(String("Hello World").swapcase(), "hELLO wORLD", "swapcase");
    TEST_EQUAL(String("hello world").title(), "Hello World", "title");

    // Stripping
    TEST_EQUAL(String("  \t hello world \n ").strip(), "hello world", "strip");
    TEST_EQUAL(String("  hello world  ").lstrip(), "hello world  ", "lstrip");
    TEST_EQUAL(String("  hello world  ").rstrip(), "  hello world", "rstrip");
    TEST_EQUAL(String("xxhelloxx").strip("x"), "hello", "strip custom chars");

    // Prefix & Suffix removal
    TEST_EQUAL(String("TestHook").removeprefix("Test"), "Hook", "removeprefix");
    TEST_EQUAL(String("TestHook").removesuffix("Hook"), "Test", "removesuffix");

    // Replace
    TEST_EQUAL(String("one, two, one, three").replace("one", "ten"), "ten, two, ten, three", "replace all");
    TEST_EQUAL(String("one, two, one, three").replace("one", "ten", 1), "ten, two, one, three", "replace count 1");

    // Justification & Padding
    TEST_EQUAL(String("42").zfill(5), "00042", "zfill positive");
    TEST_EQUAL(String("-42").zfill(5), "-0042", "zfill negative");
    TEST_EQUAL(String("test").center(10, '*'), "***test***", "center");
    TEST_EQUAL(String("test").ljust(8, '-'), "test----", "ljust");
    TEST_EQUAL(String("test").rjust(8, '-'), "----test", "rjust");

    // Expandtabs
    TEST_EQUAL(String("a\tb\tc").expandtabs(4), "a   b   c", "expandtabs 4");
}

void test_split_and_join() {
    using namespace pystring;

    // Whitespace split (Python default)
    String line = "  apples   oranges   bananas  ";
    auto tokens = line.split();
    TEST_EQUAL(tokens.size(), 3, "split whitespace size");
    TEST_EQUAL(tokens[0], "apples", "split whitespace [0]");
    TEST_EQUAL(tokens[1], "oranges", "split whitespace [1]");
    TEST_EQUAL(tokens[2], "bananas", "split whitespace [2]");

    // Delimiter split
    String csv = "1,2,3,4,5";
    auto csv_parts = csv.split(",");
    TEST_EQUAL(csv_parts.size(), 5, "csv split size");
    TEST_EQUAL(csv_parts[2], "3", "csv split [2]");

    // maxsplit
    auto limited = csv.split(",", 2);
    TEST_EQUAL(limited.size(), 3, "maxsplit size");
    TEST_EQUAL(limited[2], "3,4,5", "maxsplit remainder");

    // rsplit
    auto r_limited = csv.rsplit(",", 2);
    TEST_EQUAL(r_limited.size(), 3, "rsplit maxsplit size");
    TEST_EQUAL(r_limited[0], "1,2,3", "rsplit head remainder");
    TEST_EQUAL(r_limited[1], "4", "rsplit [1]");
    TEST_EQUAL(r_limited[2], "5", "rsplit [2]");

    // splitlines
    String multiline = "line1\nline2\r\nline3";
    auto lines = multiline.splitlines();
    TEST_EQUAL(lines.size(), 3, "splitlines size");
    TEST_EQUAL(lines[0], "line1", "splitlines [0]");
    TEST_EQUAL(lines[1], "line2", "splitlines [1]");
    TEST_EQUAL(lines[2], "line3", "splitlines [2]");

    // Join
    std::vector<std::string> fruits = {"Apple", "Banana", "Cherry"};
    String joined = String(", ").join(fruits);
    TEST_EQUAL(joined, "Apple, Banana, Cherry", "join vector");

    String joined_init = String("-").join({"A", "B", "C"});
    TEST_EQUAL(joined_init, "A-B-C", "join initializer list");
}

void test_formatting() {
    using namespace pystring;

    // Positional {}
    String s1 = format("Hello, {}! You have {} points.", "Pouya", 100);
    TEST_EQUAL(s1, "Hello, Pouya! You have 100 points.", "format positional");

    // Numbered {0}, {1}
    String s2 = format("{0} is {1}, {0} is great!", "C++", "fast");
    TEST_EQUAL(s2, "C++ is fast, C++ is great!", "format numbered");

    // Named mapping
    String s3 = format_map("Welcome {user}, role: {role}", {{"user", "admin"}, {"role", "superuser"}});
    TEST_EQUAL(s3, "Welcome admin, role: superuser", "format named map");

    // Member format
    String s4 = String("Coordinates: {}, {}").format(12.5, 45.8);
    TEST_EQUAL(s4, "Coordinates: 12.500000, 45.800000", "member format float");
}

void test_operators_and_chaining() {
    using namespace pystring;

    // Repetition *
    String s = "Py";
    TEST_EQUAL(s * 3, "PyPyPy", "s * 3");
    TEST_EQUAL(3 * s, "PyPyPy", "3 * s");

    // Addition +
    String s2 = s + "String" + "Lib";
    TEST_EQUAL(s2, "PyStringLib", "concatenation");

    // Method chaining
    String chained = String("   Hello, World!   ")
                        .strip()
                        .lower()
                        .replace("world", "C++")
                        .upper();
    TEST_EQUAL(chained, "HELLO, C++!", "method chaining");
}

void test_legacy_compatibility() {
    PyString py;
    std::string input = "pouya";

    TEST_EQUAL(py.count("banana", "an"), 2, "legacy count");
    TEST_EQUAL(py.find(input, "uya"), 2, "legacy find");
    TEST_EQUAL(py.revers(input), "ayuop", "legacy revers");
    TEST_EQUAL(py.removeVowels(input), "py", "legacy removeVowels");
    TEST_EQUAL(py.slice(input, 1, 4, 1), "ouy", "legacy slice");
    std::string rep = "xyz";
    TEST_EQUAL(py.replace("pouya", "ou", rep), "pxyzya", "legacy replace");
}

void test_utf8() {
    using namespace pystring;

    String fa = "سلام دنیا"; // 9 Unicode characters
    TEST_EQUAL(fa.utf8_len(), 9, "utf8_len Persian");
    TEST_ASSERT(fa.is_valid_utf8(), "is_valid_utf8");

    String emojis = "🌟🚀🎉";
    TEST_EQUAL(emojis.utf8_len(), 3, "utf8_len Emojis");

    // UTF-8 slice
    TEST_EQUAL(fa.utf8_slice(0, 4), "سلام", "utf8_slice 0..4");
    TEST_EQUAL(fa.utf8_slice(5, 9), "دنیا", "utf8_slice 5..9");

    // UTF-8 reverse
    String word = "سلام";
    TEST_EQUAL(word.utf8_reverse(), "مالس", "utf8_reverse");

    // utf8_chars
    auto chars = emojis.utf8_chars();
    TEST_EQUAL(chars.size(), 3, "utf8_chars count");
    TEST_EQUAL(chars[0], "🌟", "utf8_chars[0]");
    TEST_EQUAL(chars[1], "🚀", "utf8_chars[1]");
    TEST_EQUAL(chars[2], "🎉", "utf8_chars[2]");
}

void test_case_conversions() {
    using namespace pystring;

    TEST_EQUAL(String("userFirstName").to_snake_case(), "user_first_name", "to_snake_case camel");
    TEST_EQUAL(String("User-First-Name").to_snake_case(), "user_first_name", "to_snake_case kebab");
    TEST_EQUAL(String("user_first_name").to_camel_case(), "userFirstName", "to_camel_case snake");
    TEST_EQUAL(String("user_first_name").to_kebab_case(), "user-first-name", "to_kebab_case snake");
    TEST_EQUAL(String("user_first_name").to_pascal_case(), "UserFirstName", "to_pascal_case snake");
}

void test_regex() {
    using namespace pystring;

    String s = "Order 12345 confirmed";
    TEST_ASSERT(s.search_regex(R"(\d+)"), "search_regex found");
    TEST_ASSERT(String("12345").matches(R"(\d+)"), "matches digits");
    TEST_ASSERT(!String("12345a").matches(R"(\d+)"), "matches fail");

    String replaced = s.replace_regex(R"(\d+)", "[NUM]");
    TEST_EQUAL(replaced, "Order [NUM] confirmed", "replace_regex");

    String csv = "apple, orange; banana   grape";
    auto tokens = csv.split_regex(R"([\s,;]+)");
    TEST_EQUAL(tokens.size(), 4, "split_regex tokens size");
    TEST_EQUAL(tokens[0], "apple", "split_regex [0]");
    TEST_EQUAL(tokens[1], "orange", "split_regex [1]");
    TEST_EQUAL(tokens[2], "banana", "split_regex [2]");
    TEST_EQUAL(tokens[3], "grape", "split_regex [3]");

    String text = "cat, bat, rat, mat";
    auto matches = text.findall(R"([cbr]at)");
    TEST_EQUAL(matches.size(), 3, "findall size");
    TEST_EQUAL(matches[0], "cat", "findall[0]");
    TEST_EQUAL(matches[1], "bat", "findall[1]");
    TEST_EQUAL(matches[2], "rat", "findall[2]");
}

void test_algo() {
    using namespace pystring;

    // Levenshtein & Similarity
    TEST_EQUAL(String::levenshtein("kitten", "sitting"), 3, "levenshtein kitten sitting");
    TEST_EQUAL(String("hello").levenshtein("hello"), 0, "levenshtein identical");
    TEST_ASSERT(String("kitten").similarity("sitting") > 0.5, "similarity ratio");
    TEST_EQUAL(String("hello").similarity("hello"), 1.0, "similarity 1.0");

    // Base64
    String original = "Hello, Modern C++!";
    String b64 = original.to_base64();
    TEST_EQUAL(b64, "SGVsbG8sIE1vZGVybiBDKysh", "to_base64");
    TEST_EQUAL(String::from_base64(b64), original, "from_base64");

    // Hex
    String hex = String("Hello").to_hex();
    TEST_EQUAL(hex, "48656c6c6f", "to_hex");
    TEST_EQUAL(String::from_hex("48656c6c6f"), "Hello", "from_hex");
}

int main() {
    std::cout << "\033[1;34m========================================\033[0m\n";
    std::cout << "\033[1;34m     PyStringLib Comprehensive Tests    \033[0m\n";
    std::cout << "\033[1;34m========================================\033[0m\n";

    test_slicing();
    test_predicates();
    test_search();
    test_transforms();
    test_split_and_join();
    test_formatting();
    test_operators_and_chaining();
    test_legacy_compatibility();
    test_utf8();
    test_case_conversions();
    test_regex();
    test_algo();

    std::cout << "\n\033[1;32mPassed: " << tests_passed << "\033[0m | ";
    if (tests_failed > 0) {
        std::cout << "\033[1;31mFailed: " << tests_failed << "\033[0m\n";
        return 1;
    } else {
        std::cout << "\033[1;32mFailed: 0 (All tests passed successfully!)\033[0m\n";
        return 0;
    }
}
