# LP3 Core

This is a base library used by other components of LP3. On it's own, it's pretty boring, but does a few things:

* Defines some compiler symbols to detect what platform code is running on (see le.hpp).
* Similarly has code for raising errors or calling std::abort if exceptions are disabled, and creates a base exception class for other libraries to extend.
* Establishes a rudimentary logging system compatable with SDL2's log system and [the Squirrel Console](https://github.com/TimSimpson/SquirrelConsole). It also uses boost::format to support a printf syntax and completely dissapears outside of debug builds.
* Defines some compiler symbols to detect what platform code is running on (see le.hpp).
* Basic RAII helper classes to deal with SDL2 resources.
* A class to abstract the platform loop via a passed in std::function.
* A media class to load files and resources.
