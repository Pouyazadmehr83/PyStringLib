#include "PyString.h"

// PyStringLib is header-only by default.
// This translation unit ensures clean compilation for static/shared library targets.
namespace pystring {
    // Force instantiation of primary templates and linkage verification
    static_assert(sizeof(String) > 0, "String class defined correctly");
}
