#ifndef LP3_CORE_CASTS_HPP
#define LP3_CORE_CASTS_HPP
#pragma once

#include "core/config.hpp"
#include <boost/numeric/conversion/cast.hpp>
#include "le.hpp"

namespace lp3 {

// ----------------------------------------------------------------------------
// narrow - Narrow Conversion Cast
// ----------------------------------------------------------------------------
//     These macros and functions help with narrowing conversions by casting
//     them correctly. They also have checks to ensure no data is lost in
//     debug builds.
// ----------------------------------------------------------------------------
template<typename R, typename A>
inline R narrow(A number) {
    #ifdef LP3_COMPILE_WITH_DEBUGGING
        //  Just use Boost's solution to this problem LOL
        const R r = boost::numeric_cast<R>(number);
        LP3_ASSERT(r == static_cast<R>(number));
    #endif
	// Make sure this always runs.
	return static_cast<R>(number);
}

}

#endif
