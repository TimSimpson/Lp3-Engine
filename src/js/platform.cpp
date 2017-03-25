#include <GL/glfw.h>
///#include <emscripten/emscripten.h>
#include <emscripten.h>

#include <boost/scope_exit.hpp>
#include <lp3/assert.hpp>

#include <lp3/platform.hpp>

namespace lp3 { namespace core {

namespace {
    static int global_instances = 0;

    static std::function<bool()> * global_iterate = nullptr;

    void do_loop() {
        if (global_iterate) {
            (*global_iterate)();  // return value is ignored
        }
    }

}

PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
    LP3_ASSERT_TRUE(global_instances < 1);
    ++global_instances;

    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }

}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

int PlatformLoop::run(std::function<bool()> iterate) {
    BOOST_SCOPE_EXIT(void) {
        global_iterate = nullptr;
    } BOOST_SCOPE_EXIT_END
    if (iterate) {
        global_iterate = &iterate;
    }
    emscripten_set_main_loop(do_loop, 0, 1);


    return 0;
}

}   }
