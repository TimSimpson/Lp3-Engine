#include <lp3/platform.hpp>

namespace lp3 { namespace core {

PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }
}

PlatformLoop::PlatformLoop()
	: arguments()
{
}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

int PlatformLoop::run(std::function<bool()> iterate) {
    if (iterate) {
        while(iterate()) {
        }
    }
    return 0;
}

}   }
