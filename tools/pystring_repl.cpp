#include "pystring/pystring.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace repl {

using namespace pystring;

// ANSI Colors
constexpr const char* COLOR_RESET   = "\033[0m";
constexpr const char* COLOR_CYAN    = "\033[1;36m";
constexpr const char* COLOR_GREEN   = "\033[1;32m";
constexpr const char* COLOR_YELLOW  = "\033[1;33m";
constexpr const char* COLOR_RED     = "\033[1;31m";
constexpr const char* COLOR_MAGENTA = "\033[1;35m";
constexpr const char* COLOR_BLUE    = "\033[1;34m";
constexpr const char* COLOR_GRAY    = "\033[0;90m";

std::map<std::string, String> variables;

void print_banner() {
    std::cout << COLOR_CYAN;
    std::cout << R"(
  ____        ____  _        _             _     _ _     
 |  _ \ _   _/ ___|| |_ _ __(_)_ __   __ _| |   (_) |__  
 | |_) | | | \___ \| __| '__| | '_ \ / _` | |   | | '_ \
 |  __/| |_| |___) | |_| |  | | | | | (_| | |___| | |_) |
 |_|    \__, |____/ \__|_|  |_|_| |_|\__, |_____|_|_.__/
        |___/                        |___/               
)" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << "  Pythonic String Manipulation for Modern C++ (Interactive REPL)\n" << COLOR_RESET;
    std::cout << COLOR_GRAY << "  Type 'help' for examples, 'vars' for variables, 'exit' or Ctrl+D to quit.\n\n" << COLOR_RESET;
}

void print_help() {
    std::cout << COLOR_YELLOW << "\nAvailable Commands & Syntax:\n" << COLOR_RESET;
    std::cout << "  " << COLOR_GREEN << "s = \"hello world\"" << COLOR_RESET << "          Assign a string variable\n";
    std::cout << "  " << COLOR_GREEN << "s.upper()" << COLOR_RESET << "                    Convert to uppercase\n";
    std::cout << "  " << COLOR_GREEN << "s.lower()" << COLOR_RESET << "                    Convert to lowercase\n";
    std::cout << "  " << COLOR_GREEN << "s.title()" << COLOR_RESET << "                    Convert to Title Case\n";
    std::cout << "  " << COLOR_GREEN << "s.strip()" << COLOR_RESET << "                    Strip whitespace from ends\n";
    std::cout << "  " << COLOR_GREEN << "s.to_snake_case()" << COLOR_RESET << "            Convert to snake_case\n";
    std::cout << "  " << COLOR_GREEN << "s.to_camel_case()" << COLOR_RESET << "            Convert to camelCase\n";
    std::cout << "  " << COLOR_GREEN << "s.to_kebab_case()" << COLOR_RESET << "            Convert to kebab-case\n";
    std::cout << "  " << COLOR_GREEN << "s.to_pascal_case()" << COLOR_RESET << "           Convert to PascalCase\n";
    std::cout << "  " << COLOR_GREEN << "s.utf8_len()" << COLOR_RESET << "                 Unicode code points count\n";
    std::cout << "  " << COLOR_GREEN << "s.to_base64()" << COLOR_RESET << "                Encode to Base64\n";
    std::cout << "  " << COLOR_GREEN << "s.to_hex()" << COLOR_RESET << "                   Encode to Hexadecimal\n";
    std::cout << "  " << COLOR_GREEN << "s.levenshtein(\"target\")" << COLOR_RESET << "    Levenshtein edit distance\n";
    std::cout << "  " << COLOR_GREEN << "s.similarity(\"target\")" << COLOR_RESET << "     Similarity ratio (0.0 .. 1.0)\n";
    std::cout << "  " << COLOR_GREEN << "s.matches(\"regex\")" << COLOR_RESET << "          Regex fullmatch check\n";
    std::cout << "  " << COLOR_GREEN << "s[::-1]" << COLOR_RESET << "                      Python reverse slice\n";
    std::cout << "  " << COLOR_GREEN << "s[1:5]" << COLOR_RESET << "                       Python slice range\n";
    std::cout << "  " << COLOR_GREEN << "s[-1]" << COLOR_RESET << "                        Access last character\n";
    std::cout << "  " << COLOR_GREEN << "s * 3" << COLOR_RESET << "                        Repeat string 3 times\n";
    std::cout << "  " << COLOR_GREEN << "vars" << COLOR_RESET << "                         List stored variables\n";
    std::cout << "  " << COLOR_GREEN << "clear" << COLOR_RESET << "                        Clear terminal screen\n";
    std::cout << "  " << COLOR_GREEN << "exit / quit" << COLOR_RESET << "                  Exit REPL\n\n";
}

void print_vars() {
    if (variables.empty()) {
        std::cout << COLOR_GRAY << "  (No variables defined yet)\n" << COLOR_RESET;
        return;
    }
    std::cout << COLOR_YELLOW << "Defined Variables:\n" << COLOR_RESET;
    for (const auto& [name, val] : variables) {
        std::cout << "  " << COLOR_CYAN << name << COLOR_RESET << " = \"" << COLOR_GREEN << val << COLOR_RESET << "\"\n";
    }
    std::cout << "\n";
}

String parse_literal(std::string_view token) {
    token = pystring::strip(token);
    if ((token.front() == '"' && token.back() == '"') ||
        (token.front() == '\'' && token.back() == '\'')) {
        return String(token.substr(1, token.size() - 2));
    }
    auto it = variables.find(std::string(token));
    if (it != variables.end()) {
        return it->second;
    }
    return String(token);
}

void evaluate(String line) {
    line = line.strip();
    if (line.empty()) return;

    if (line == "exit" || line == "quit") {
        std::cout << COLOR_GRAY << "Goodbye!\n" << COLOR_RESET;
        std::exit(0);
    }
    if (line == "help") {
        print_help();
        return;
    }
    if (line == "clear") {
        std::cout << "\033[2J\033[1;1H";
        print_banner();
        return;
    }
    if (line == "vars") {
        print_vars();
        return;
    }

    // 1. Variable Assignment: var = "value"
    if (line.contains("=") && !line.contains("==")) {
        auto [var_name, eq, val_expr] = line.partition("=");
        String name = var_name.strip();
        String val = val_expr.strip();

        if (name.isidentifier()) {
            variables[std::string(name)] = parse_literal(val);
            std::cout << COLOR_GRAY << "=> " << COLOR_CYAN << name << COLOR_RESET 
                      << " = \"" << COLOR_GREEN << variables[std::string(name)] << COLOR_RESET << "\"\n";
            return;
        }
    }

    // 2. Repetition: s * 3
    if (line.contains("*")) {
        auto [lhs, star, rhs] = line.partition("*");
        String s = parse_literal(lhs.strip());
        ptrdiff_t count = 1;
        try {
            count = std::stoll(std::string(rhs.strip()));
            std::cout << COLOR_GREEN << "\"" << (s * count) << "\"\n" << COLOR_RESET;
            return;
        } catch (...) {}
    }

    // 3. Slicing: var[start:stop:step]
    if (line.contains("[") && line.endswith("]")) {
        auto [var_part, brk, slice_part] = line.partition("[");
        String var_name = var_part.strip();
        String slice_content = slice_part.removesuffix("]").strip();

        String target = parse_literal(var_name);

        if (!slice_content.contains(":")) {
            // Negative or single index: s[-1]
            try {
                ptrdiff_t idx = std::stoll(std::string(slice_content));
                std::cout << COLOR_YELLOW << "'" << target[idx] << "'\n" << COLOR_RESET;
                return;
            } catch (...) {}
        } else {
            // Python slice syntax: [start:stop:step]
            auto parts = slice_content.split(":");
            std::optional<ptrdiff_t> start, stop, step;
            if (parts.size() >= 1 && !parts[0].empty()) start = std::stoll(std::string(parts[0]));
            if (parts.size() >= 2 && !parts[1].empty()) stop = std::stoll(std::string(parts[1]));
            if (parts.size() >= 3 && !parts[2].empty()) step = std::stoll(std::string(parts[2]));

            String res = target(start, stop, step);
            std::cout << COLOR_GREEN << "\"" << res << "\"\n" << COLOR_RESET;
            return;
        }
    }

    // 4. Method calls: target.method(...)
    if (line.contains(".")) {
        auto [target_str, dot, method_call] = line.partition(".");
        String target = parse_literal(target_str.strip());
        method_call = method_call.strip();

        if (method_call.startswith("upper(")) {
            std::cout << COLOR_GREEN << "\"" << target.upper() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("lower(")) {
            std::cout << COLOR_GREEN << "\"" << target.lower() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("title(")) {
            std::cout << COLOR_GREEN << "\"" << target.title() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("strip(")) {
            std::cout << COLOR_GREEN << "\"" << target.strip() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("capitalize(")) {
            std::cout << COLOR_GREEN << "\"" << target.capitalize() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_snake_case(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_snake_case() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_camel_case(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_camel_case() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_kebab_case(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_kebab_case() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_pascal_case(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_pascal_case() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("utf8_len(")) {
            std::cout << COLOR_BLUE << target.utf8_len() << " code points\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_base64(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_base64() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("to_hex(")) {
            std::cout << COLOR_GREEN << "\"" << target.to_hex() << "\"\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("split(")) {
            auto tokens = target.split();
            std::cout << COLOR_YELLOW << "[";
            for (size_t i = 0; i < tokens.size(); ++i) {
                std::cout << "\"" << tokens[i] << "\"" << (i + 1 < tokens.size() ? ", " : "");
            }
            std::cout << "]\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("levenshtein(")) {
            String arg = std::get<2>(method_call.partition("(")).removesuffix(")").strip();
            String other = parse_literal(arg);
            std::cout << COLOR_BLUE << target.levenshtein(other) << "\n" << COLOR_RESET;
            return;
        }
        if (method_call.startswith("similarity(")) {
            String arg = std::get<2>(method_call.partition("(")).removesuffix(")").strip();
            String other = parse_literal(arg);
            std::cout << COLOR_BLUE << target.similarity(other) << "\n" << COLOR_RESET;
            return;
        }
    }

    // Default literal evaluation
    String res = parse_literal(line);
    std::cout << COLOR_GREEN << "\"" << res << "\"\n" << COLOR_RESET;
}

} // namespace repl

int main() {
    repl::print_banner();

    // Prepopulate helpful demo variables
    repl::variables["s"] = "  Hello, Modern C++ World!  ";
    repl::variables["fa"] = "سلام دنیا 🌟";

    std::string input;
    while (true) {
        std::cout << repl::COLOR_CYAN << ">>> " << repl::COLOR_RESET;
        if (!std::getline(std::cin, input)) {
            std::cout << "\n" << repl::COLOR_GRAY << "Exiting...\n" << repl::COLOR_RESET;
            break;
        }
        try {
            repl::evaluate(input);
        } catch (const std::exception& e) {
            std::cerr << repl::COLOR_RED << "Error: " << e.what() << repl::COLOR_RESET << "\n";
        }
    }
    return 0;
}
