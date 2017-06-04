#define SDL_MAIN_HANDLED
#include <iostream>

#include <lp3/log.hpp>
#include <lp3/core.hpp>
#include <lp3/main.hpp>


namespace core = lp3::core;


int _main(core::PlatformLoop & loop) {
    lp3::core::LogSystem log;

    LP3_LOG_DEBUG("Greetings from the MicroMain Demo.");
    for(const std::string & element : loop.command_line_args()) {
        LP3_LOG_DEBUG(element.c_str());
        #ifdef LP3_COMPILE_LOG_DISABLE
            std::cout << element << "\n";
        #endif
    }

    const auto result = loop.run([&]() {
        // This gets called each frame.
		return false;
    });

    LP3_LOG_DEBUG("Good bye.");
    return result;
}

LP3_MAIN(_main)
