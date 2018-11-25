
# This CMake super project uses Conan.
# By placing Conan support here, it keeps the root project clean and portable
# between package managers.

project(lp3_engine-Parent CXX)
cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)


# Note: This project is using the "cmake" Conan generator.
# Using "cmake_paths" is a bit cleaner, but you have to explicitly call Conan
# with `-DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake` (for some reason, that won't
# happen automatically when you use `conan build`.
# I'm also not 100% certain the two approaches are the same, so I'm doing it
# this way for now.
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

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
