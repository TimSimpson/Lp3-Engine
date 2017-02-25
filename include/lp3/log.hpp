#ifndef FILE_LP3_LOG_HPP
#define FILE_LP3_LOG_HPP

#include "core/config.hpp"

#include "le.hpp"

namespace lp3 { namespace core {
    enum class LogLevel
    {
        Critical,
        Debug,
        Error,
        Info,
        Warning
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

    #define LP3_LOG_CRITICAL(m, ...) /* m */
    #define LP3_LOG_DEBUG(m, ...) /* m */
    #define LP3_LOG_ERROR(m, ...) /* m */
    #define LP3_LOG_INFO(m, ...) /* m */
    #define LP3_LOG_WARNING(m, ...) /* m */
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

    #define LP3_LOG(l, m, ...) \
        lp3::core::write_log(__FILE__, __LINE__, l, m, ##__VA_ARGS__);
    #define LP3_LOG_CRITICAL(m, ...) LP3_LOG(lp3::core::LogLevel::Critical, m,  ##__VA_ARGS__);
    #define LP3_LOG_DEBUG(m, ...) LP3_LOG(lp3::core::LogLevel::Debug, m,  ##__VA_ARGS__);
    #define LP3_LOG_INFO(m, ...) LP3_LOG(lp3::core::LogLevel::Info, m,  ##__VA_ARGS__);
    #define LP3_LOG_ERROR(m, ...) LP3_LOG(lp3::core::LogLevel::Error, m,  ##__VA_ARGS__);
    #define LP3_LOG_WARNING(m, ...) LP3_LOG(lp3::core::LogLevel::Warning, m,  ##__VA_ARGS__);

    #define LP3_LOG_SET(name, value) \
    {   \
        LP3_LOG_DEBUG("%s@%s", name, value); \
    }


#endif
#endif
