// ----------------------------------------------------------------------------
// assert.hpp
// ----------------------------------------------------------------------------
//      Defines LP3_ASSERT, which is almost identical to SDL_assert, but can
//      be configured to raise exceptions which allows for testing that
//      asserts are triggered.
// ---------------------------------------------------------------------------/
#ifndef FILE_LP3_ASSERT_H
#define FILE_LP3_ASSERT_H
#pragma once

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
// ----------------------------------------------------------------------------
// LP3_ASSERT
// ----------------------------------------------------------------------------
//      This defers almost entirely to SDL_assert to the point that
//      it is nearly equivalent, except it can be configured to throw
//      exceptions on assertion failure, making it possible to write
//      test cases that negative code paths are properly terminated
//      in debug mode. Basically, the only reason to use this over
//      SDL_assert is to make code more testable (and theoretically
//      if an assert is triggered in debug it means the calling code is
//      not honoring a contract, making it undefined behavior and not
//      a part of the code's interface; personally I still want to
//      verify these asserts are triggered even if it's only in debug
//      mode).
// ----------------------------------------------------------------------------
    #define LP3_ASSERT(condition)   \
        if (!(condition)) { \
            lp3::core::AssertListener::throw_assert_failed(); \
        } \
        SDL_assert(condition)

// end-doc

// ----------------------------------------------------------------------------
// LP3_REQUIRE_ASSERT_FAILURE
// ----------------------------------------------------------------------------
//      Proves that some bit of code will trigger an assertion at runtime.
//      This only works in debug mode (meaning that test code you write here
//      only executes in debug mode).
// ----------------------------------------------------------------------------

    #define LP3_REQUIRE_ASSERT_FAILURE(code) \
        {   \
            ::lp3::core::AssertListener listener; \
            REQUIRE_THROWS_AS(code, ::lp3::core::AssertFailed); \
        }
// end-doc

#else
    #define LP3_ASSERT(condition)   \
        SDL_assert(condition)
    #define LP3_REQUIRE_ASSERT_FAILURE(code) \
        while (SDL_NULL_WHILE_LOOP_CONDITION) { code; }
#endif



#endif
