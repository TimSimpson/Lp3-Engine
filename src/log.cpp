#define LP3_CORE_API_CREATE
#include <lp3/log.hpp>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <lp3/core/utils.hpp>


#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include "ErrOut.hpp"
#endif
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h>
#endif
#if defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    #include <stdio.h>
#endif
#include <SDL.h>

namespace lp3 { namespace core {

namespace {
	static LogLevel verbosity_level = LogLevel::Debug;

	const char * log_level_to_text(const LogLevel & level) {
		switch (level) {
		case LogLevel::Critical:
			return "CRIT  ";
		case LogLevel::Error:
			return "ERROR ";
		case LogLevel::Warning:
			return "WARN  ";
		case LogLevel::Info:
			return "INFO  ";
		case LogLevel::Debug:
			return "DEBUG ";
		default:
			return "?!";
		};
	}

	void _write_log_stdout(const char * const filename, const int lineNumber,
		                   const LogLevel & level, const char * const message)
	{
		if (level <= verbosity_level) {
			::printf("%s %s %d : ",
				     log_level_to_text(level), filename, lineNumber);
			::printf("%s", message);
			::printf("\n");
		}
	}
}

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
		_write_log_stdout(filename, lineNumber, level, message);
    }
#endif

#if defined(LP3_COMPILE_TARGET_DREAMCAST) \
    || defined(LP3_COMPILE_TARGET_LINUX) \
    || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    void write_log(const char * const filename, const int lineNumber,
                   const LogLevel & level, const char * const message)
    {
		_write_log_stdout(filename, lineNumber, level, message);
    }
#endif

namespace {

#ifdef LP3_COMPILE_TARGET_WINDOWS
	LogLevel sdl_priority_to_log_level(SDL_LogPriority priority) {
		switch (priority) {
		case SDL_LOG_PRIORITY_VERBOSE:
		case SDL_LOG_PRIORITY_DEBUG:
			return LogLevel::Debug;
		case SDL_LOG_PRIORITY_INFO:
			return LogLevel::Info;
		case SDL_LOG_PRIORITY_WARN:
			return LogLevel::Warning;
		case SDL_LOG_PRIORITY_ERROR:
			return LogLevel::Error;
		case SDL_LOG_PRIORITY_CRITICAL:
			return LogLevel::Critical;
		default:
			return LogLevel::Debug;
		}

	}
	const char * sdl_category_to_filename(int category) {
		switch (category) {
			case SDL_LOG_CATEGORY_APPLICATION:
				return "SDL_LOG_CATEGORY_APPLICATION";
			case SDL_LOG_CATEGORY_ERROR:
				return "SDL_LOG_CATEGORY_ERROR";
			case SDL_LOG_CATEGORY_ASSERT:
				return "SDL_LOG_CATEGORY_ASSERT";
			case SDL_LOG_CATEGORY_SYSTEM:
				return "SDL_LOG_CATEGORY_SYSTEM";
			case SDL_LOG_CATEGORY_AUDIO:
				return "SDL_LOG_CATEGORY_AUDIO";
			case SDL_LOG_CATEGORY_VIDEO:
				return "SDL_LOG_CATEGORY_VIDEO";
			case SDL_LOG_CATEGORY_RENDER:
				return "SDL_LOG_CATEGORY_RENDER";
			case SDL_LOG_CATEGORY_INPUT:
				return "SDL_LOG_CATEGORY_INPUT";
			case SDL_LOG_CATEGORY_TEST:
				return "SDL_LOG_CATEGORY_TEST";
			case SDL_LOG_CATEGORY_CUSTOM:
				return "SDL_LOG_CATEGORY_CUSTOM";
			default:
				return "? ? !";
		}
	}

	void sdl_log(void * /* userdata */, int category, SDL_LogPriority priority,
		         const char * message) {
		const char * category_msg = sdl_category_to_filename(category);
		LogLevel level = sdl_priority_to_log_level(priority);
		write_log(category_msg, 0, level, message);
	}
#endif

}

LP3_CORE_API
LogSystem::LogSystem()
{
    #ifndef LP3_COMPILE_LOG_DISABLE
        #ifdef LP3_COMPILE_TARGET_WINDOWS
            ::ErrOutPipe::Init();
		    SDL_LogSetOutputFunction(sdl_log, nullptr);
        #endif
		auto level = lp3::core::get_env_var("LP3_LOG_LEVEL");
		if (level) {
			std::string l = level.get();
			std::transform(l.begin(), l.end(), l.begin(), ::tolower);
			verbosity_level =
				(l == "critical" ? LogLevel::Critical
					: (l == "error" ? LogLevel::Error
						: (l == "warning" ? LogLevel::Warning
							: (l == "info" ? LogLevel::Info
								: LogLevel::Debug))));
		}
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
