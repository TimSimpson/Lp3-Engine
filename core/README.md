# LP3 Core

This is a base library used by other components of LP3. On it's own, it's pretty boring, but does a few things:

* Defines some compiler symbols to detect what platform code is running on (see le.hpp).

* Similarly has code for raising errors or calling std::abort if exceptions are disabled, and creates a base exception class for other libraries to extend.

* Establishes a rudimentary logging system compatable with SDL2's log system and [the Squirrel Console](https://github.com/TimSimpson/SquirrelConsole). It also uses boost::format to support a printf syntax and completely dissapears outside of debug builds.

* Defines some compiler symbols to detect what platform code is running on (see le.hpp).

* Basic RAII helper classes to deal with SDL2 resources.

* A class to abstract the platform loop via a passed in std::function.

* A media class to load files and resources.

Here's an example of the simplest possible Lp3 app. It sets up the logging system, prints some stuff out, and quits:

    #include <iostream>

    #include <lp3/log.hpp>
    #include <lp3/core.hpp>
    #include <lp3/main.hpp>

    namespace core = lp3::core;


    int _main(core::PlatformLoop & loop) {
        lp3::core::LogSystem log;

        LP3_LOG_DEBUG("Greetings from the MicroMain Demo.");
        int index = 0;
        for(const std::string & element : loop.command_line_args()) {
            LP3_LOG_ERROR("%d. %s", index, element);
            ++ index;
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

For examples of how to use this stuff, see [the demos directory](demos).

## Build Info

If you're using CMake, just include core's CMakeLists.txt as a subdirectory. It will export the library Lp3_Core.

## Library Options

<dl>
    <dt>LP3_CORE_API_DYNAMIC</dt>
    <dd>If set, forces library functions to be defined as `__declspec(dllexport)` on Windows. Is set automatically when BUILD_SHARED_LIBS is set.</dd>


    <dt>LP3_COMPILE_LOG_DISABLE</dt>
    <dd>If set, disables logging even in debug mode. Good if you don't wish to use the built-in logging utilities.</dd>
</dl>

