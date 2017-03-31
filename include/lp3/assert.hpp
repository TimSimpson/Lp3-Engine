#ifndef FILE_LP3_ASSERT_H
#define FILE_LP3_ASSERT_H

#include "core/config.hpp"
#include "core/Exception.hpp"
#define SDL_MAIN_HANDLED
#include <SDL.h>

// SDL_assert *almost* did what I wanted, but oops, it's crappy old C code
// so there's no way to optionally raise an exception, which is what you
// want to be able to do in test code. So I have to wrap it.

// Using this it's possible to make the assert raise an exception, which means
// test code can assert a code path raises an assert. It's also good when the
// assert might stop bad code which can potentially crash the test program from
// executing, since the exception will stop the code path without aborting the
// entire program.

namespace lp3 { namespace core {

// Instantiate this class to make all failed asserts throw exceptions.
LP3_CORE_API
class AssertListener {
public:
    AssertListener();
    ~AssertListener();
    static void throw_assert_failed();
};

// In test code, check for this exception type to be thrown.
LP3_CORE_API
class AssertFailed : public lp3::core::Exception {
public:
	AssertFailed();
};


}   }

#ifdef LP3_COMPILE_WITH_DEBUGGING
    #define LP3_ASSERT(condition)   \
        if (!condition) { \
            lp3::core::AssertListener::throw_assert_failed(); \
        } \
        SDL_assert(condition)
#else
    #define LP3_ASSERT(condition)   \
        SDL_assert(condition)
#endif


#endif
