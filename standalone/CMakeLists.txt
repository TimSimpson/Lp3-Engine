
# A parent project which tests that the main CMakeLists.txt can be used
# as a subproject.
#
# This is used only for CI; in your own project, ignore this and add lp3_core as
# a subproject (nested directory) using Git submodules, ExternalProject_Add or
# something else.
#
# Note that you will need to define the libraries below as seen here yourself.

project(lp3_engine-Parent CXX)
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

add_library(lp3_deps::gsl INTERFACE IMPORTED)
set_property(TARGET lp3_deps::gsl PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${DEPS_PATH}/gsl/include")


### Boost

find_package(Boost REQUIRED)
add_library(lp3_deps::boost INTERFACE IMPORTED)
set_property(TARGET lp3_deps::boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}")


### SDL2

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DEPS_PATH}/cmake")

set(SDL2 "${DEPS_PATH}/sdl2")
find_package(SDL2 REQUIRED)
if (NOT "${SDL2_FOUND}")
    message(FATAL_ERROR "Could not find SDL libraries.")
endif()
add_library(lp3_deps::sdl2 INTERFACE IMPORTED)
set_property(TARGET lp3_deps::sdl2 PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${SDL2_INCLUDE_DIR}")
set_property(TARGET lp3_deps::sdl2 PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${SDL2_LIBRARY}")


### SDL2_IMAGE

# Add SDL2 image library, then add it to SDL2
find_library(SDL2_image NAMES "SDL2_image"
    PATHS
    "${DEPS_PATH}/sdl2/lib"
    "/usr/lib/x86_64-linux-gnu")

add_library(lp3_deps::sdl2_image INTERFACE IMPORTED)

set_property(TARGET lp3_deps::sdl2_image PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${SDL2_image}")


### SDL2_TTF

# Add SDL2 ttf library, then add it to SDL2
find_library(SDL2_ttf NAMES "SDL2_ttf"
    PATHS
    "${DEPS_PATH}/sdl2/lib"
    "/usr/lib/x86_64-linux-gnu")

add_library(lp3_deps::sdl2_ttf INTERFACE IMPORTED)

set_property(TARGET lp3_deps::sdl2_ttf PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${SDL2_ttf}")


### SDL2_MIXER

if("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    add_library(sdlx_mixer INTERFACE IMPORTED)
else()
    find_library(SDL2_mixer NAMES "SDL2_mixer"
        PATHS
        "${DEPS_PATH}/sdl2/lib"
        "/usr/lib/x86_64-linux-gnu")

    add_library(lp3_deps::sdlx_mixer INTERFACE IMPORTED)

    set_property(TARGET lp3_deps::sdlx_mixer PROPERTY
        INTERFACE_LINK_LIBRARIES
        "${SDL2_mixer}")
endif()



### Catch

add_library(lp3_deps::catch INTERFACE IMPORTED)
set_property(TARGET lp3_deps::catch PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${DEPS_PATH}/Catch/single_include")


### GLM

add_library(lp3_deps::glm INTERFACE IMPORTED)
set_property(TARGET lp3_deps::glm PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${DEPS_PATH}/glm")

###############################################################################
#  Subproject
###############################################################################

set(LP3_CORE_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core
                 ${CMAKE_CURRENT_BINARY_DIR}/core-output)

set(LP3_SIMS_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../sims
                 ${CMAKE_CURRENT_BINARY_DIR}/sims-output)

set(LP3_GL_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../gl
                 ${CMAKE_CURRENT_BINARY_DIR}/gl-output)

set(LP3_GFX_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../gfx
                 ${CMAKE_CURRENT_BINARY_DIR}/gfx-output)

set(LP3_IMGUI_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../imgui
                 ${CMAKE_CURRENT_BINARY_DIR}/imgui-output)

set(LP3_INPUT_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../input
                 ${CMAKE_CURRENT_BINARY_DIR}/input-output)

set(LP3_MIX_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../mix
                 ${CMAKE_CURRENT_BINARY_DIR}/mix-output)