#!/usr/bin/env python3
"""
Amalgamation script for PyStringLib.
Generates single_include/pystring.hpp from separate modular headers.
"""

import os
import re

HEADER_FILES = [
    "slice.hpp",
    "predicates.hpp",
    "search.hpp",
    "split_join.hpp",
    "ops.hpp",
    "format.hpp",
    "core.hpp",
    "pystring.hpp"
]

def amalgamate(include_dir, output_file):
    std_includes = set()
    content_blocks = []

    std_include_regex = re.compile(r'^\s*#include\s*<([^>]+)>')
    local_include_regex = re.compile(r'^\s*#include\s*"([^"]+)"')
    pragma_once_regex = re.compile(r'^\s*#pragma\s+once')
    ifndef_regex = re.compile(r'^\s*#\s*(ifndef|define|endif)(\s+|(\s*//\s*))PYSTRING_[A-Za-z0-9_]+_HPP\b')

    for fname in HEADER_FILES:
        fpath = os.path.join(include_dir, fname)
        if not os.path.exists(fpath):
            continue
        with open(fpath, "r", encoding="utf-8") as f:
            lines = f.readlines()

        cleaned_lines = []
        for line in lines:
            if pragma_once_regex.match(line) or ifndef_regex.match(line):
                continue
            m_std = std_include_regex.match(line)
            if m_std:
                std_includes.add(m_std.group(1))
                continue
            if local_include_regex.match(line):
                continue
            cleaned_lines.append(line)

        content_blocks.append(f"// --- Begin: {fname} ---\n" + "".join(cleaned_lines) + f"\n// --- End: {fname} ---\n")

    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, "w", encoding="utf-8") as out:
        out.write("""/**
 * PyStringLib - Single-Header Amalgamated Edition
 * Pythonic String Library for Modern C++ (C++17 / C++20 / C++23)
 * 
 * GitHub: https://github.com/Pouyazadmehr83/PyStringLib
 * License: MIT
 */

#ifndef PYSTRING_SINGLE_HEADER_HPP
#define PYSTRING_SINGLE_HEADER_HPP

""")
        out.write("// Standard Library Dependencies\n")
        for inc in sorted(std_includes):
            out.write(f"#include <{inc}>\n")
        out.write("\n")

        for block in content_blocks:
            out.write(block)
            out.write("\n")

        out.write("#endif // PYSTRING_SINGLE_HEADER_HPP\n")

    print(f"[+] Amalgamated single-header generated at: {output_file}")

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    root_dir = os.path.abspath(os.path.join(script_dir, ".."))
    include_dir = os.path.join(root_dir, "include", "pystring")
    output_file = os.path.join(root_dir, "single_include", "pystring.hpp")
    amalgamate(include_dir, output_file)
