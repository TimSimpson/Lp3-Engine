#ifndef FILE_LP3_PLATFORM_HPP
#define FILE_LP3_PLATFORM_HPP

// Handles platform specific code. Only include this after you include
// windows.h or it will fail.
#include <functional>
#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <lp3/le.hpp>
#include <lp3/core/utils.hpp>

namespace lp3 { namespace core {

// --------------------------------------------------------------------
// class PlatformLoop
// --------------------------------------------------------------------
//     Contains code to run the main loop on a given platform.
//     This class is not type-erased but doesn't need to be as a main
//     unit will have to be aware of platform specific arguments.
//     main.hpp has code to create the main loop and return this.
// --------------------------------------------------------------------

class PlatformLoop {
public:
    PlatformLoop(int argc, char ** argv);
	PlatformLoop();
    PlatformLoop(const PlatformLoop & other) = delete;
    PlatformLoop & operator=(const PlatformLoop & other) = delete;

    std::vector<std::string> command_line_args() const;

    int run(std::function<bool()> iterate);

private:
    std::vector<std::string> arguments;
};

}   }

#endif
