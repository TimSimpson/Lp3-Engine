# LP3 Core

[![Build status](https://ci.appveyor.com/api/projects/status/a3r2jq32f1x6frcv?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-core)

This is a base library used by other components of LP3. It does a few things:

* Sets up macros to help determine details about the platform where code is running.

* Establishes a rudimentary logging system (that can be disabled).

* Creates type erased interfaces to the underlying platform (this allows for other libraries to avoid including the dastardly <windows.h> in their headers).

* Some utility classes for keeping time or running third-rate coroutines which may get moved into another project soon.

For examples of how to use this stuff, see [the demos directory](demos).

## Building

This library is built with CMake, and tries to adhere to the some rules described in [The Ultimate Guide to Modern CMake](https://rix0r.nl/blog/2015/08/13/cmake-guide/). Specifically effort is made to avoid trying to find it's own dependencies so that they can be specified by a parent project instead.

For an example of what this looks like please see [the parent project used for CI](standalone/CMakeLists.txt).

### Dependencies / Requirements

* [The Boost libraries](http://www.boost.org/) (header only) defined as the imported interface library `boost`.
* The [Guideline Support Library] (https://github.com/Microsoft/GSL), defined as the imported interface library `gsl`.

### Options

This describes various compiler flags and options that your parent project can set before including this one.

<dl>
    <dt>LP3_CORE_API_DYNAMIC</dt>
    <dd>If set, forces library functions to be defined as `__declspec(dllexport)` on Windows. Is set automatically when BUILD_SHARED_LIBS is set.</dd>


    <dt>LP3_COMPILE_LOG_DISABLE</dt>
    <dd>If set, disables logging even in debug mode. Good if you don't wish to use the built-in logging utilities.</dd>
</dl>

### Exports

This project will export the library `Lp3_Core` which can be used by subsequent projects.
