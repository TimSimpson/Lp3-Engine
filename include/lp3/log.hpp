#ifndef FILE_LP3_LOG_HPP
#define FILE_LP3_LOG_HPP

#include "core/config.hpp"

#include "le.hpp"

namespace lp3 { namespace core {
    enum class LogLevel
    {
        Critical=0,
        Debug=4,
        Error=1,
        Info=3,
        Warning=2
    };

    class
    LP3_CORE_API
    LogSystem
    {
    public:
        LogSystem();
        ~LogSystem();
    };
} }

#ifdef LP3_COMPILE_LOG_DISABLE

    #define LP3_LOG_CRITICAL(...) /* */
    #define LP3_LOG_DEBUG(...) /* */
    #define LP3_LOG_ERROR(...) /* */
    #define LP3_LOG_INFO(...) /* */
    #define LP3_LOG_WARNING(...) /* */
    #define LP3_LOG_SET(name, value) /* name, value */

#else
    #include <boost/format.hpp>

    namespace lp3 { namespace core {

        template<typename LastType>
        void feedToFormat(boost::format & format, const LastType & arg)
        {
            format % arg;
        }

        template<typename HeadType, typename... TailTypes>
        void feedToFormat(boost::format & format, const HeadType & head,
                          const TailTypes... tail)
        {
            format % head;
            feedToFormat(format, tail...);
        }

        LP3_CORE_API
        void write_log(const char * const filename, const int lineNumber,
                      const LogLevel & level, const char * const message);


        template<typename... ParameterTypes>
        void write_log(const char * const filename, const int lineNumber,
                      const LogLevel & level, const char * const formatString,
                      const ParameterTypes... args)
        {
            auto format = boost::format(formatString);
            feedToFormat(format, args...);
            const auto message = str(format);
            write_log(filename, lineNumber, level, message.c_str());
        }

    } }

    #define LP3_LOG(...) \
        lp3::core::write_log(__FILE__, __LINE__, __VA_ARGS__);
    #define LP3_LOG_CRITICAL(...) LP3_LOG(lp3::core::LogLevel::Critical , __VA_ARGS__);
    #define LP3_LOG_DEBUG(...) LP3_LOG(lp3::core::LogLevel::Debug, __VA_ARGS__);
    #define LP3_LOG_INFO(...) LP3_LOG(lp3::core::LogLevel::Info, __VA_ARGS__);
    #define LP3_LOG_ERROR(...) LP3_LOG(lp3::core::LogLevel::Error, __VA_ARGS__);
    #define LP3_LOG_WARNING(...) LP3_LOG(lp3::core::LogLevel::Warning, __VA_ARGS__);

    #define LP3_LOG_SET(name, value) \
    {   \
        LP3_LOG_DEBUG("%s@%s", name, value); \
    }


#endif
#endif
