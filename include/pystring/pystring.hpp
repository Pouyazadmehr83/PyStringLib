#ifndef PYSTRING_PYSTRING_HPP
#define PYSTRING_PYSTRING_HPP

/**
 * @file pystring.hpp
 * @brief Main umbrella header for PyStringLib.
 * 
 * PyStringLib is a modern C++ library bringing Python's powerful and elegant
 * string manipulation capabilities to C++17/20/23 with zero overhead,
 * method chaining, zero-copy string views, UTF-8 awareness, and Pythonic syntax.
 * 
 * Developed by Pouya Zadmehr (Pouyazadmehr83).
 */

#include "slice.hpp"
#include "predicates.hpp"
#include "search.hpp"
#include "split_join.hpp"
#include "ops.hpp"
#include "format.hpp"
#include "utf8.hpp"
#include "case_conv.hpp"
#include "regex_ops.hpp"
#include "algo.hpp"
#include "core.hpp"

namespace pystring {
    // Pythonic alias: pystring::str
    using str = String;
}

#endif // PYSTRING_PYSTRING_HPP
