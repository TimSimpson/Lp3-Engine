/************************************************************************/
/*  Assert Methods                                                      */
/*  In release mode, these are ignored. In debug builds, they crash the */
/*  the program. In test mode, they are test cases.                     */
/************************************************************************/
#ifndef FILE_LP3_ASSERT_H
#define FILE_LP3_ASSERT_H

#include <lp3/le.hpp>
#ifdef LP3_COMPILE_WITH_DEBUGGING
#include <lp3/log.hpp>
#include <stdlib.h>
#endif

//LP3_ASSERT_EQUAL
//  Confirm the first argument equals the second.

//LP3_ASSERT_NOT_EQUAL
//  Confirm the first argument does not equal the second.

//LP3_ASSERT_NOT_EQUAL_MESSAGE
//  Confirm the first argument does not equals the second, outputs a message
//  on failure.

//LP3_ASSERT_TRUE
//  The first argument is a boolean and must be true. The second is a message.

//LP3_ASSERT_TRUE_MESSAGE
//  The first argument is a boolean and must be true. The second is a message.

#if defined(LP3_COMPILE_TARGET_TEST) || defined(BOOST_TEST_MODULE)
    #if defined(LP3_COMPILE_TARGET_USE_BOOST_TEST)
        #define LP3_ASSERT_EQUAL(a, b) BOOST_REQUIRE_EQUAL(a, b)
        #define LP3_ASSERT_EQUAL_MESSAGE(a, b, m) BOOST_REQUIRE_MESSAGE(a == b, m)
        #define LP3_ASSERT_NOT_EQUAL(a, b) BOOST_REQUIRE_PREDICATE( _1 != _2, (a)(b))
        #define LP3_ASSERT_NOT_EQUAL_MESSAGE(a, b, m) BOOST_REQUIRE_MESSAGE((a != b), (m))
        #define LP3_ASSERT_TRUE(a) BOOST_CHECK(a)
        #define LP3_ASSERT_TRUE_MESSAGE(a, m) BOOST_REQUIRE_MESSAGE(a, m)
        #define LP3_ASSERT_THROW(expr, type) \
            BOOST_CHECK_THROW(expr, type)
    #else
        #include <lp3/test_asserts.hpp>

        #define LP3_ASSERT_EQUAL(a, b) \
            lp3::asserts::assert_equal(__FILE__, __LINE__, a, b);
        #define LP3_ASSERT_EQUAL_MESSAGE(a, b, m) \
            lp3::asserts::assert_equal(__FILE__, __LINE__, a, b, m);
        #define LP3_ASSERT_NOT_EQUAL(a, b) \
            lp3::asserts::assert_not_equal(__FILE__, __LINE__, a, b);
        #define LP3_ASSERT_NOT_EQUAL_MESSAGE(a, b, m) \
            lp3::asserts::assert_not_equal(__FILE__, __LINE__, a, b, m);
        #define LP3_ASSERT_TRUE(a) \
            lp3::asserts::assert_true(__FILE__, __LINE__, (a), nullptr);
        #define LP3_ASSERT_TRUE_MESSAGE(a, m) \
            lp3::asserts::assert_true(__FILE__, __LINE__, (a), m);
        #ifndef LP3_COMPILE_TARGET_DREAMCAST
            #define LP3_ASSERT_THROW(expr, type) \
                try { { expr ; } \
                    LP3_ASSERT_EQUAL_MESSAGE(1, 2, "Expected to see thrown exception."); \
                } catch(const type exception) { }
        #else
            // Dreamcast:
            #define LP3_ASSERT_THROW(expr, type) \
                if (! setjmp(lp3::asserts::buf)) { \
                    { expr; } \
                    LP3_ASSERT_EQUAL_MESSAGE(1, 2, "Expected to see thrown exception."); \
                } else {                     \
                    LP3_ASSERT_EQUAL(LP3_THROW_EXCEPTION_TO_STRING(type), \
                                     lp3::asserts::last_exception_type); \
                }
        #endif
    #endif
#else
    #ifdef LP3_COMPILE_WITH_DEBUGGING
        #define LP3_ASSERT_EQUAL(a, b) \
        {   \
            if (a != b) \
            { \
                LP3_LOG_CRITICAL("Assert failed."); \
                abort(); \
            } \
        }

        #define LP3_ASSERT_EQUAL_MESSAGE(a, b, m) \
        {   \
            if (a != b) \
            {   \
                LP3_LOG_CRITICAL("Assert failed (message to follow):"); \
                LP3_LOG_CRITICAL(m); \
                abort(); \
            }   \
        }

        #define LP3_ASSERT_NOT_EQUAL(a, b) \
        {   \
            if (a == b) \
            {   \
                LP3_LOG_CRITICAL("Assert failed."); \
                abort(); \
            }   \
        }

        #define LP3_ASSERT_NOT_EQUAL_MESSAGE(a, b, m) \
        {   \
            if (a != b) \
            {   \
                LP3_LOG_CRITICAL("Assert failed (message to follow):"); \
                LP3_LOG_CRITICAL(m); \
                abort(); \
            }   \
        }

        #define LP3_ASSERT_TRUE(a) \
        {   \
            if (!(a))   \
            {   \
                LP3_LOG_CRITICAL("Assert failed."); \
                abort(); \
            }   \
        }

        #define LP3_ASSERT_TRUE_MESSAGE(a, m)   \
        {   \
            if (!(a))   \
            {   \
                LP3_LOG_CRITICAL("Assert failed (message to follow):"); \
                LP3_LOG_CRITICAL(m); \
                abort(); \
            }   \
        }

    #else
        #define LP3_ASSERT_EQUAL(a, b) { /* a, b */ }
        #define LP3_ASSERT_EQUAL_MESSAGE(a, b, m) { /* a, b, m */ }
        #define LP3_ASSERT_NOT_EQUAL(a, b) { /* a, b */ }
        #define LP3_ASSERT_NOT_EQUAL_MESSAGE(a, b, m) { /* a, b, m */ }
        #define LP3_ASSERT_TRUE(a) { /* a, m */ }
        #define LP3_ASSERT_TRUE_MESSAGE(a, m) { /* a, m */ }
    #endif
#endif


#define LP3_ASSERT_FALSE(a) LP3_ASSERT_TRUE(!(a));
#define LP3_ASSERT_FALSE_MESSAGE(a, m) LP3_ASSERT_TRUE_MESSAGE(!(a), m);

#define LP3_FAIL(m) LP3_ASSERT_TRUE_MESSAGE(false, m);


#endif // File Include Guard
