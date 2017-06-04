#ifndef LP3_INPUT_CONFIG_HPP

#define LP3_INPUT_CONFIG_HPP
#pragma once

#include <lp3/le.hpp>

// Similar to lp3/core/config.hpp. See there for details.

#if defined(LP3_COMPILE_DYNAMIC) || defined(LP3_INPUT_API_DYNAMIC)
# if defined(LP3_INPUT_API_CREATE)
#  define LP3_INPUT_API LP3_SYMBOL_EXPORT
# else
#  define LP3_INPUT_API LP3_SYMBOL_IMPORT
# endif
#else
# define LP3_INPUT_API
#endif

#endif
