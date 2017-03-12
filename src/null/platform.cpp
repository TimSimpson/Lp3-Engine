#include <lp3/assert.hpp>
#include <lp3/platform.hpp>
#include "../platform.ipp"

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

int PlatformLoop::run(std::function<bool()> iterate,
                      boost::optional<std::function<void(PlatformMessage)>>) {
    if (iterate) {
        while(iterate()) {
        }
    }
    return 0;
}

}   }
