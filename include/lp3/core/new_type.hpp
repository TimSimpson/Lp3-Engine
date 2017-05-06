#ifndef LP3_CORE_STRONG_TYPEDEF_HPP
#define LP3_CORE_STRONG_TYPEDEF_HPP
# pragma once


// Code is modified version of BOOST_STRONG_TYPEDEF from the Boost libraries
// (specifically boost/serialization/strong_typedef.hpp)
// Modified to forward constructor arguments and not require certain
// operators, with some tricks from Imperfect C++'s' (Copyright Matthew Wilson,
// 2005) True Typedefs mixed in.

namespace lp3 { namespace core {

template<typename T, typename U>
class NewType {
public:
    T t;

    template<typename... Params>
    explicit NewType(Params&&... args)
    : t(std::forward<Params>(args)...)
    {}

    // explicit NewType(const NewType & other) : t(other.t){}

    NewType & operator=(const NewType & rhs) {
        t = rhs.t;
        return *this;
    }

    NewType & operator=(const T & rhs) {
        t = rhs;
        return *this;
    }

    operator const T & () const {return t; }

    operator T & () { return t; }
};

}   }


#define LP3_NEW_TYPE_SUBCLASS_CTOR(NAME)         \
   template<typename... Params>                  \
   explicit NAME(Params&&... args)               \
    : TrueTypedef(std::forward<Params>(args)...) \
    {}

#define LP3_NEW_TYPE_PLUS(NAME, ORIGINAL_TYPE, NEW_TYPE_ADAPTER)   \
    class NAME ## ___OpaqueType {};                                \
    using NAME = NEW_TYPE_ADAPTER<ORIGINAL_TYPE, NAME ## ___OpaqueType>;

#define LP3_NEW_TYPE(NAME, ORIGINAL_TYPE)                       \
    LP3_NEW_TYPE_PLUS(NAME, ORIGINAL_TYPE, ::lp3::core::NewType)


#endif
