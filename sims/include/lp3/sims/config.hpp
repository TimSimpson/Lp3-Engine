#ifndef LP3_SIMS_CONFIG_HPP
#define LP3_SIMS_CONFIG_HPP

#include <lp3/le.hpp>

// Creates macros needed to create portable library code.
//
// Macros:
//
// LP3_SIMS_API_DYNAMIC
//     If defined, this code will be built or used as a library.
//     If not defined, then the code will be compiled into the resulting binary.
//
// LP3_SIMS_API_CREATE
//     If defined, we are building this library.  If not, we are using it.
//
// LP3_SIMS_API
//     This is defined by this header file by looking at the above values.
//     This may or may not become a compiler annotation affixed to all symbol
//     definitions.  This is necessary for certain compilers (specifically MSVC++).
//

#if defined(LP3_COMPILE_DYNAMIC) || defined(LP3_SIMS_API_DYNAMIC)
# if defined(LP3_SIMS_API_CREATE)
#  define LP3_SIMS_API LP3_SYMBOL_EXPORT
# else
#  define LP3_SIMS_API LP3_SYMBOL_IMPORT
# endif
#else
# define LP3_SIMS_API
#endif

#endif
