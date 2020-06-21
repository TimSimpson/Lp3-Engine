
# This CMake super project uses Conan.
# By placing Conan support here, it keeps the root project clean and portable
# between package managers.

project(lp3_engine-Parent CXX)
cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 17)
enable_testing()
include(ExternalProject)

# Hack a bunch of stuff to make it work with Conan.

# This project was written to work with Cget packages. For Conan support, this
# means here we need to:
#
#   1. Add header only libraries to the global include include_directories
#   2. make aliases of the important targets from what Conan named them to
#      what the other packages did.


function(add_header_library SRC)
    get_property(var TARGET ${SRC} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    include_directories(${var})
endfunction()

function(clone_library DST SRC)
    add_library(${DST} INTERFACE IMPORTED)

    get_property(var TARGET ${SRC} PROPERTY INTERFACE_LINK_LIBRARIES)
    set_property(TARGET ${DST} PROPERTY INTERFACE_LINK_LIBRARIES ${var})

    get_property(var TARGET ${SRC} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    set_property(TARGET ${DST} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${var})

    get_property(var TARGET ${SRC} PROPERTY INTERFACE_COMPILE_DEFINITIONS)
    set_property(TARGET ${DST} PROPERTY INTERFACE_COMPILE_DEFINITIONS ${var})

    get_property(var TARGET ${SRC} PROPERTY INTERFACE_COMPILE_OPTIONS)
    set_property(TARGET ${DST} PROPERTY INTERFACE_COMPILE_OPTIONS ${var})
endfunction()


find_package(gsl_microsoft REQUIRED)
add_header_library(gsl_microsoft::gsl_microsoft)

find_package(Catch2 REQUIRED)
add_header_library(Catch2::Catch2)

find_package(glm REQUIRED)
clone_library(glm glm::glm)

find_package(sdl2 REQUIRED)
# clone_library(sdl2::sdl2 CONAN_PKG::sdl2)  # oh the irony, I named mine the same
find_package(sdl2_image REQUIRED)
clone_library(sdl2::image sdl2_image::sdl2_image)
find_package(sdl2_ttf REQUIRED)
clone_library(sdl2::ttf sdl2_ttf::sdl2_ttf)
find_package(sdl2_mixer REQUIRED)
clone_library(sdl2::mixer sdl2_mixer::sdl2_mixer)


# Load this, as Core can't seem to "export" it
find_package(lp3-sdl REQUIRED)


# Do the find_package macro trick to ignore any package we're dealing with
# specially, such as subprojects or Conan-derived dependencies.

set(fp_ignore glm sdl2 Lp3_Core Lp3_Sims Lp3_Gl Lp3_Gfx Lp3_ImGui Lp3_Input Lp3_Mix)

macro(find_package )
    if(NOT ${ARGV0} IN_LIST fp_ignore)
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

set(LP3_MIX_FULL true)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../mix
                 ${CMAKE_CURRENT_BINARY_DIR}/mix-output)
