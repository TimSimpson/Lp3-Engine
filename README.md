# LP3 Core

This is a base library used by other components of LP3. It does a few things:

    * Sets up macros to help determine where code is running.

    * Establishes a rudimentary logging system (that can be disabled).

    * Creates type erased interfaces to the underlying platform (to allows for the other libraries to avoid including the dastardly <windows.h> in their headers).

## Building

This library is built with CMake, and tries to adhere to the some rules described in [The Ultimate Guide to Modern CMake](https://rix0r.nl/blog/2015/08/13/cmake-guide/). Specifically effort is made to avoid trying to find it's own dependencies so that they can be specified by a parent project instead. However there is limited support for building it stand-alone as well.

### Dependencies / Requirements

This library uses the Boost libraries (header only). As a result, `boost` must be defined as an imported interface library, like so:

    find_package(Boost REQUIRED)
    add_library(boost INTERFACE IMPORTED)
    set_property(TARGET boost PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}")

Additionally, this library requires the [Guideline Support Library] (https://github.com/Microsoft/GSL) (again, header only). You'll need to define `gsl` to be a library as an imported interface library like so:

    add_library(gsl INTERFACE IMPORTED)
    set_property(TARGET gsl PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES "/path/to/gsl")

### Using as a Subproject

To add this to another CMake project, just place it in the same directory (using git submodules or other means) and add some code along the lines of

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/path/to/lp3-core)

### Exports

This project will export the library `Lp3_Core` which can be used by subsequent projects.

### Options

    LP3_CORE_API_DYNAMIC - If set, forces library functions to be defined as `__declspec(dllexport)` on Windows.

    LP3_COMPILE_LOG_DISABLE - If set, disables logging even in debug mode.

