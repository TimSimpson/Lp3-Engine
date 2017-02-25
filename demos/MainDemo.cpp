#include <lp3/log.hpp>
#include <lp3/core.hpp>
#include <lp3/main.hpp>


namespace core = lp3::core;


int _main(core::PlatformLoop & loop) {
    lp3::core::LogSystem log;

    LP3_LOG_DEBUG("Greetings from the MicroMain Demo.");
    for(const std::string & element : loop.command_line_args())
    {
        LP3_LOG_DEBUG(element.c_str());
    }

    const auto result = loop.run([&]() {
        // This gets called each frame.
    },
    [&](core::PlatformMessage msg) {
        // This is an optional second argument that is called for specific
        // events from the underlying platform (such as resizing Windows).
    });

    LP3_LOG_DEBUG("Good bye.");
    return result;
}

LP3_MAIN(_main, "MicroMainDemo");
