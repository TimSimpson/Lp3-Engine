#define SDL_MAIN_HANDLED
#include <lp3/log.hpp>
#include <lp3/core.hpp>

int main() {
    lp3::core::LogSystem log;

    LP3_LOG_DEBUG("Hello Squirrel!");
    LP3_LOG_DEBUG("Start the debug console to see this.");

    LP3_LOG_DEBUG("Writing a char string literal.");

    #ifdef LP3_COMPILE_TARGET_WINDOWS
        LP3_LOG_DEBUG("%s", L"Writing a wide string literal: ¶.");
    #endif

    LP3_LOG_WARNING("Writing a WARNING message.");

    LP3_LOG_ERROR("Writing an ERROR message.");

    LP3_LOG_DEBUG("This is a demonstration on using Boost::Format.");
    LP3_LOG_DEBUG(str(boost::format("%s=%d") % "The Answer" % 42).c_str());

    LP3_LOG_SET("Squirrel", "CRAZY");
    LP3_LOG_SET("The Answer", "42");
}
