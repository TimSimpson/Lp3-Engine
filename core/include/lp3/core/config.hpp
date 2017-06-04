#ifndef LP3_CORE_CONFIG_HPP
#define LP3_CORE_CONFIG_HPP
#pragma once

#include <lp3/le.hpp>

// Creates macros needed to create portable library code.
//
// Macros:
//
// LP3_CORE_API_DYNAMIC
//     If defined, this code will be built or used as a library.
//     If not defined, then the code will be compiled into the resulting binary.
//
// LP3_CORE_API_CREATE
//     If defined, we are building this library.  If not, we are using it.
//
// LP3_CORE_API
//     This is defined by this header file by looking at the above values.
//     This may or may not become a compiler annotation affixed to all symbol
//     definitions.  This is necessary for certain compilers (specifically MSVC++).
//

#if defined(LP3_COMPILE_DYNAMIC) || defined(LP3_CORE_API_DYNAMIC)
# if defined(LP3_CORE_API_CREATE)
#  define LP3_CORE_API LP3_SYMBOL_EXPORT
# else
#  define LP3_CORE_API LP3_SYMBOL_IMPORT
# endif
#else
# define LP3_CORE_API
#endif

#endif
