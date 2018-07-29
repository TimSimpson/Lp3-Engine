#ifndef LP3_SIMS_COROUTINE_HPP
#define LP3_SIMS_COROUTINE_HPP

#include "config.hpp"
#include <lp3/core.hpp>

namespace lp3 { namespace sims {

// ---------------------------------------------------------------------------
// Simple Coroutines
// =================
//
// lp3::sims coroutines are stateless coroutines which don't persist the values
// of local variables between invocations (like Lua or Boost coroutines) but
// are much cheaper are portable. They work by using a macro to create a
// simple state machine with switch statements. The result is nicer than the
// you might expect.
//
// Rules for using coroutines:
//
//  - Function variables will not retain values in between calls, so always
//    use class variables instead.
//  - Use independent CoroutineState variables for each coroutine. Don't muck
//    with the internal variables of CoroutineState.
//  - Only use one pair of begin / end coroutine macros per function.
//
// The ideas which led to this are fairly well known and I wrote this
// implementation based this on my own research. However I feel the need to
// credit Simon Tatham for writing a paper that details a method nearly
// identical to what is used here.
//
// http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
//
// ~see-file "../../../tests/coroutine_tests.cpp"
// --------------------------------------------------------------------------/

// ~end-doc summary

// ---------------------------------------------------------------------------
// class CoroutineState
// ---------------------------------------------------------------------------
//      Stores a code pointer used to implement the coroutine.
// ---------------------------------------------------------------------------
class
LP3_SIMS_API
CoroutineState
{
public:
    // This is declared public, but only because there was no other (simple)
    // choice. It's best to leave this alone.
    int code_pointer;

    CoroutineState()
    :   code_pointer(0)
    {
    }

    // Returns true if the coroutine is still running.
    explicit inline operator bool() const {
        return code_pointer != -1;
    }

    // Go to a place marked with
    inline void go_to(int label) {
        LP3_ASSERT(label > 0);
        code_pointer = label * -1 - 1;
    }

};

class
LP3_SIMS_API
CoroutineFinishedException : public lp3::core::Exception
{
public:
    CoroutineFinishedException() BOOST_NOEXCEPT
    :   Exception("Coroutine was called again after completing.")
    {
    }
};

class
LP3_SIMS_API
CoroutineInvalidStateException : public lp3::core::Exception
{
public:
    CoroutineInvalidStateException() BOOST_NOEXCEPT
    :   Exception("The Coroutine is in a corrupted or invalid state.")
    {
    }
};


} }


// ---------------------------------------------------------------------------
// LP3_COROUTINE_BEGIN(coroutine_state)
// ---------------------------------------------------------------------------
//      Pass this an instance of the CoroutineState class at the top of your
//      function.
// ---------------------------------------------------------------------------
#define LP3_COROUTINE_BEGIN(variable_name) \
    int & _lp3_coroutine_code_pointer = variable_name.code_pointer; \
    switch(_lp3_coroutine_code_pointer) \
    { \
        case(0):

// ---------------------------------------------------------------------------
// LP3_COROUTINE_END()
// ---------------------------------------------------------------------------
//      Put this at the bottom of your coroutine function.
// ---------------------------------------------------------------------------
#define LP3_COROUTINE_END() \
            _lp3_coroutine_code_pointer = -1; \
            break; \
        case(-1): \
            LP3_LOG_ERROR("Coroutine has already ended!"); \
            LP3_THROW(::lp3::sims::CoroutineFinishedException); \
            break; \
        default: \
            LP3_LOG_ERROR("Coroutine is in an invalid state!"); \
            LP3_THROW(::lp3::sims::CoroutineInvalidStateException); \
            break; \
    }

// ~end-doc

#define LP3_YIELDL(VALUE, ...) \
            _lp3_coroutine_code_pointer=VALUE; \
            return __VA_ARGS__;                     \
        case(VALUE):


// ---------------------------------------------------------------------------
// LP3_YIELD(...)
// ---------------------------------------------------------------------------
//      Suspends the coroutine and (optionally) returns a value to the
//      caller.
// ---------------------------------------------------------------------------
#define LP3_YIELD(...) LP3_YIELDL(__LINE__, __VA_ARGS__)

// ---------------------------------------------------------------------------
// LP3_LABEL(label_name)
// ---------------------------------------------------------------------------
//      Creates a label that will work in conjunction with the coroutine's
//      yield statement. That's right: GOTO is back baby! And better than ever.
// ---------------------------------------------------------------------------
#define LP3_LABEL(VALUE) \
            _lp3_coroutine_code_pointer=(VALUE) * -1 -1; \
        case((VALUE) * -1 -1):

// ~end-doc

#endif
