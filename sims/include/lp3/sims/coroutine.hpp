#ifndef LP3_SIMS_COROUTINE_HPP
#define LP3_SIMS_COROUTINE_HPP

#include "config.hpp"
#include <lp3/core.hpp>

namespace lp3 { namespace sims {

/*
 * Simple Coroutines
 * -----------------
 *
 * This code allows for coroutines that are crappier than Lua (and Boost)
 * coroutines, but about as good as Python's and portable.
 *
 * Rules for using coroutines:
 *
 *  - Function variables will not retain values in between calls, so always
 *    use class variables instead.
 *  - Use independent CoroutineState variables for each coroutine. Don't muck
 *    with the internal variables of CoroutineState.
 *  - Only use one pair of begin / end coroutine macros per function.
 *
 * See the unit tests for examples.
 *
 * The ideas which led to this are fairly well known and based on independent
 * research; however I feel the need to credit Simon Tatham for writing a paper
 * that details a method nearly identical to what is used here.
 *
 * http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
 */
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


#define LP3_COROUTINE_BEGIN(variable_name) \
    int & _lp3_coroutine_code_pointer = variable_name.code_pointer; \
    switch(_lp3_coroutine_code_pointer) \
    { \
        case(0):

#define LP3_LABELL(LABEL_NAME, VALUE) \
        LABEL_NAME: \
            _lp3_coroutine_code_pointer=VALUE; \
        case(VALUE):


#define LP3_YIELDL(VALUE, ...) \
            _lp3_coroutine_code_pointer=VALUE; \
            return __VA_ARGS__;                     \
        case(VALUE):


#define LP3_LABEL(LABEL_NAME) LP3_LABELL(LABEL_NAME, __LINE__)
#define LP3_YIELD(...) LP3_YIELDL(__LINE__, __VA_ARGS__)

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

#endif
