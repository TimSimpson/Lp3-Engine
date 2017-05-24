# A parent project which tests that the main CMakeLists.txt can be used
# as a subproject.
#
# This is used only for CI; in your own project, ignore this and add Lp3-Core as
# a subproject (nested directory) using Git submodules, ExternalProject_Add or
# something else.
#
# Note that you will need to define the libraries below as seen here yourself.

project(Lp3-Core-Parent CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compile_options(/WX)
else()
    add_definitions("-Wall -pedantic -Werror -Wshadow -Wno-long-long -Wextra")
endif()

# if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     #set devloper compiler options
#     #ALL developers should build in debug mode by default
#     # set(CMAKE_CXX_FLAGS "-Wall -Werror -Wshadow -Wno-long-long -Wextra ${CMAKE_CXX_FLAGS}")
#     # set(CMAKE_C_FLAGS "-Wall -Werror -Wshadow -Wno-long-long -Wextra ${CMAKE_C_FLAGS}")
# endif()

# Not needed?
# if (CMAKE_CXX_COMPILER_ID MATCHES "GCC")
#     add_compile_options(-std=c++14)
# endif()
set (CMAKE_CXX_STANDARD 14)

enable_testing()

include(ExternalProject)

###############################################################################
#  Third Party libraries
###############################################################################

# If you are using this as a subproject, your super project CMakeLists.txt
# must define the following interface import libraries as seen here.

set(DEPS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/deps")

### Guidelines Support Library

add_library(gsl INTERFACE IMPORTED)
set_property(TARGET gsl PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${DEPS_PATH}/gsl/include")


### Boost

find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}")


### SDL2

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DEPS_PATH}/cmake")

set(SDL2 "${DEPS_PATH}/sdl2")
find_package(SDL2 REQUIRED)
if (NOT "${SDL2_FOUND}")
    message(FATAL_ERROR "Could not find SDL libraries.")
endif()
add_library(sdl2 INTERFACE IMPORTED)
set_property(TARGET sdl2 PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${SDL2_INCLUDE_DIR}")
set_property(TARGET sdl2 PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${SDL2_LIBRARY}")


### Catch

add_library(catch INTERFACE IMPORTED)
set_property(TARGET catch PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${DEPS_PATH}/Catch/single_include")

###############################################################################
#  Subproject
###############################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/..
                 ${CMAKE_CURRENT_BINARY_DIR}/output)