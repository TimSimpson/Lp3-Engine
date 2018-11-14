
# A parent project which tests that the main CMakeLists.txt can be used
# as a subproject.
#
# This is used only for CI; in your own project, ignore this and add lp3_core as
# a subproject (nested directory) using Git submodules, ExternalProject_Add or
# something else.
#
# Note that you will need to define the libraries below as seen here yourself.

project(lp3_engine-Parent CXX)
cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

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
set (CMAKE_CXX_STANDARD 17)

enable_testing()

include(ExternalProject)


###############################################################################
#  Subproject
###############################################################################

set(subprojects Lp3_Core Lp3_Sims Lp3_Gl Lp3_Gfx Lp3_ImGui Lp3_Input Lp3_Mix)

macro(find_package )
    if(NOT ${ARGV0} IN_LIST subprojects)
        _find_package(${ARGV})
    endif()
endmacro()

set(LP3_SETUP_FOLDERS TRUE)  # Organizes VS folders

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

# set(LP3_MIX_FULL true)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../mix
#                  ${CMAKE_CURRENT_BINARY_DIR}/mix-output)
