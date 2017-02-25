#include <GL/glfw.h>
///#include <emscripten/emscripten.h>
#include <emscripten.h>

#include <boost/scope_exit.hpp>
#include <lp3/assert.hpp>

#include <lp3/platform.hpp>

namespace lp3 { namespace core {

namespace {
    static int global_instances = 0;

    static std::function<void()> * global_iterate = nullptr;

    void do_loop() {
        if (global_iterate) {
            (*global_iterate)();
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

PlatformInitArgs PlatformLoop::platform_args() {
    return Platform::create_init_args();
}

int PlatformLoop::run(std::function<void()> iterate,
                      std::function<void(PlatformMessage)> on_message)
{
    return run(iterate, boost::optional<decltype(on_message)>(on_message));
}

int PlatformLoop::run(std::function<void()> iterate,
                      boost::optional<std::function<void(PlatformMessage)>>
                          on_message) {
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
