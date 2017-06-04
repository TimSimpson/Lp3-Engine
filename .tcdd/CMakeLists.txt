
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

{{ cmake_deps }}

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
