#include <lp3/assert.hpp>
#include <lp3/platform.hpp>

namespace lp3 { namespace core {

PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }
}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

PlatformInitArgs PlatformLoop::platform_args() {
    return Platform::create_init_args();
}

int PlatformLoop::run(std::function<void()> iterate,
                      std::function<void(PlatformMessage)> on_message)
{
    return run(iterate, boost::optional<decltype(on_message)>(on_message));
}

int PlatformLoop::run(std::function<void()> iterate,
                      boost::optional<std::function<void(PlatformMessage)>>) {
    if (iterate) {
        while(true) {
            iterate();
        }
    }
    return 0;
}

}   }
