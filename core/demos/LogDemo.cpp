// --------------------------------------------------------------------
// Log Demo
// --------------------------------------------------------------------
//      This example shows how to use the logging system.
// --------------------------------------------------------------------
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

    LP3_LOG_DEBUG("These macros use Boost::Format.");
    LP3_LOG_DEBUG("%s=%d", "The Answer", 42);
}
// ~end-doc
