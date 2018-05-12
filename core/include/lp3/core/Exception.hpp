#ifndef LP3_CORE_EXCEPTION_HPP
#define LP3_CORE_EXCEPTION_HPP
#pragma once

#include <exception>
#include <type_traits>
#include <string>
#include "config.hpp"

namespace lp3 { namespace core {

// ----------------------------------------------------------------------------
// class Exception
// ----------------------------------------------------------------------------
//     Base class for exceptions thrown by LP3. This will only be thrown
//     in debug builds or on platforms where exceptions are enabled.
//     An exception of this type is considered a fatal error- this is preferred
//     to std::abort simply to make testing and other forms of error handling
//     easier.
// ----------------------------------------------------------------------------
class
LP3_CORE_API
Exception
:   public virtual std::exception
{
public:
    template<typename Message>
    explicit Exception(
        Message && msg,
        typename std::enable_if<
            !std::is_same<Exception, Message>::value>::type * = nullptr
    ) BOOST_NOEXCEPT
    :     exception(),
        message(std::forward<Message>(msg)) {
    }

	explicit Exception() BOOST_NOEXCEPT
	:	exception(),
		message() {
	}

    Exception(Exception const  & other) BOOST_NOEXCEPT;

    Exception  & operator=(Exception const  & other);

    char const *  what() const BOOST_NOEXCEPT override;

private:
    std::string message;
};

} }

#endif
