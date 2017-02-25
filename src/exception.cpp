#define LP3_CORE_API_CREATE
#include <lp3/core/Exception.hpp>
#include <lp3/log.hpp>

namespace lp3 { namespace core {

    LP3_CORE_API
    Exception::Exception(Exception const  & other) BOOST_NOEXCEPT
    :   message(other.message)
    {
    }

    LP3_CORE_API
    Exception  & Exception::operator=(Exception const  & other)
    {
        this->message = other.message;
        return *this;
    }

    LP3_CORE_API
    char const *  Exception::what() const BOOST_NOEXCEPT
    {
        return message.c_str();
    }

} }



#ifdef BOOST_NO_EXCEPTIONS

namespace boost {
    void throw_exception(std::exception const & e) {
        LP3_LOG_ERROR("An exception occured in Boost: %s", e.what());
        abort();
    }
}

#endif
