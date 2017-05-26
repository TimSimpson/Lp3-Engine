#include <lp3/core/utils.hpp>
#include <boost/lexical_cast.hpp>


namespace lp3 { namespace core {

namespace {
    boost::optional<int> loop_count() {
        auto value = get_env_var("LP3_LOOP_COUNT");
        if (!value)
            return boost::none;
        else
            return boost::lexical_cast<int>(*value);
    }
}

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

int PlatformLoop::run(std::function<bool()> iterate) {
    if (iterate) {
        const auto count = loop_count();
        if (count) {
            int c = *count;
            while(iterate() && (--c) < 0) {}
        } else {
            while(iterate()) {}
        }
    }
    return 0;
}

}   }
