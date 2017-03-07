#define LP3_CORE_API_CREATE
#include <lp3/log.hpp>

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include "ErrOut.hpp"
#endif
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h>
#endif
#if defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    #include <stdio.h>
#endif

namespace lp3 { namespace core {

#ifdef LP3_COMPILE_TARGET_WINDOWS
    namespace {
        int priority(const LogLevel & level) {
            switch(level) {
                case LogLevel::Critical:
                    return 0;
                case LogLevel::Debug:
                    return 255;
                case LogLevel::Error:
                    return 63;
                case LogLevel::Info:
                    return 191;
                case LogLevel::Warning:
                    return 127;
                default:
                    return 0;
            }
        }
    }

	LP3_CORE_API
    void write_log(const char * const filename, const int lineNumber,
                  const LogLevel & level, const char * const message)
    {
        ::ErrOutPipe::WriteLinePL(filename, lineNumber, priority(level), message);
    }
#endif

#if defined(LP3_COMPILE_TARGET_DREAMCAST) \
    || defined(LP3_COMPILE_TARGET_LINUX) \
    || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    void write_log(const char * const filename, const int lineNumber,
                   const LogLevel &, const char * const message)
    {
        ::printf("%s %d : ", filename, lineNumber);
        ::printf("%s", message);
        ::printf("\n");
    }
#endif

LP3_CORE_API
LogSystem::LogSystem()
{
    #ifndef LP3_COMPILE_LOG_DISABLE
        #ifdef LP3_COMPILE_TARGET_WINDOWS
            ::ErrOutPipe::Init();
        #endif
    #endif // LP3_COMPILE_TARGET_WINDOWS
}

LP3_CORE_API
LogSystem::~LogSystem()
{
    #ifndef LP3_COMPILE_LOG_DISABLE
        #ifdef LP3_COMPILE_TARGET_WINDOWS
            ::ErrOutPipe::Close();
        #endif
    #endif
}

} }
