#ifndef FILE_LP3_TEST_ASSERTS_HPP
#define FILE_LP3_TEST_ASSERTS_HPP

#include <setjmp.h>
#include <lp3/le.hpp>
#include <iostream>

#if defined(LP3_COMPILE_TARGET_USE_BOOST_TEST)

    // Don't define-

#else

// Contains special assert messages for testing on platforms that don't have
// better alternatives.

namespace lp3 { namespace asserts {

extern jmp_buf buf;
extern const char * last_exception_type;
extern int error_count;

template<typename TypeA, typename TypeB>
static void assert_equal(const char * file_name, int line_number,
                               TypeA a, TypeB b,
                               const char * message=nullptr) {
    if (a == b) {
        return;
    }
    ++ error_count;
    std::cout << "@TEST_ERROR : " << file_name << ":" << line_number
              << " : \n\t" << a << " != " << b;
    if (message) {
        std::cout << " : " << message;
    }
    std::cout << std::endl;
}

template<typename TypeA, typename TypeB>
static void assert_not_equal(const char * file_name, int line_number,
                             TypeA a, TypeB b,
                             const char * message=nullptr) {
    if (a != b) {
        return;
    }
    ++ error_count;
    std::cout << "@TEST_ERROR : " << file_name << ":" << line_number
              << " : \n\t" << a << " == " << b;
    if (message) {
        std::cout << " : " << message;
    }
    std::cout << std::endl;
}

template<typename T>
static void assert_true(const char * file_name, int line_number,
                        T var, const char * message=nullptr) {
    if (var) {
        return;
    }
    ++ error_count;
    std::cout << "@TEST_ERROR : " << file_name << ":" << line_number
              << " : \n\t";
    if (message) {
        std::cout  << message;
    }
    else {
        std::cout << "Actual value was false.";
    }
    std::cout << std::endl;
}


} }

#endif
#endif
