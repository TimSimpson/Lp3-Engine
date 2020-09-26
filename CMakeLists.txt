# *********************************************************************
# LP3_ENGINE
#       Combines several bits of game / media functionality.
# *********************************************************************
cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

if(NOT DEFINED PROJECT_NAME)
    set(NOT_SUBPROJECT ON)
endif()

project(lp3_engine CXX)

include(CTest)
enable_testing()
include(ExternalProject)

# Allow user to ask explicitly to build tests
option(LP3_ENGINE_Build_Tests "Build tests when BUILD_TESTING is enabled."
       ${NOT_SUBPROJECT})

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

find_package(glm REQUIRED)
clone_library(glm glm::glm)


if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    find_package(SDL2)
    if(TARGET SDL2::SDL2-static)
        clone_library(sdl2::sdl2 SDL2::SDL2-static)
    elseif(TARGET SDL2::SDL2)
        clone_library(sdl2::sdl2 SDL2::SDL2)
    else()
        # Try using BinCrafter's find package name from pre 2.0.12 versions.
        find_package(sdl2)
        if(TARGET sdl2::sdl2)
            # cool?
        else()
            message(FATAL_ERROR "Could not find SDL2!")
        endif()
    endif()
endif()

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


# If testiing is required, turn on the "full" options for each subproject.
if(BUILD_TESTING AND LP3_ENGINE_Build_Tests)
    find_package(Catch2 REQUIRED)
    add_header_library(Catch2::Catch2)

    set(LP3_CORE_FULL true)

    set(LP3_GL_FULL true)

    set(LP3_GFX_FULL true)

    set(LP3_IMGUI_FULL true)

    set(LP3_INPUT_FULL true)

    set(LP3_MIX_FULL true)
endif()


# Do the find_package macro trick to ignore any package we're dealing with
# specially, such as subprojects or Conan-derived dependencies.

set(fp_ignore glm sdl2 Lp3_Core Lp3_Sims Lp3_Gl Lp3_Gfx Lp3_ImGui Lp3_Input Lp3_Mix)

macro(find_package )
    if(NOT ${ARGV0} IN_LIST fp_ignore)
        _find_package(${ARGV})
    endif()
endmacro()

set(LP3_SETUP_FOLDERS TRUE)  # Organizes VS folders


# # If imgui isn't there, download it.
# # NOTE: this is only to support cget- I may not keep this around.
# if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/lib/imgui.h")
#     file(DOWNLOAD
#             "https://github.com/ocornut/imgui/archive/1cd1ca259efd38d560ac3c44e6353af81a9b72e3.tar.gz"
#             "${PROJECT_BINARY_DIR}/imgui.tar.gz"
#              SHOW_PROGRESS
#              EXPECTED_MD5 "1c0d601fa39235a4e3611c206403a49a")
#     execute_process(
#         WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
#         COMMAND ${CMAKE_COMMAND} -E tar xvf "${PROJECT_BINARY_DIR}/imgui.tar.gz"
#     )
#     file(COPY
#         "${PROJECT_BINARY_DIR}/imgui-1cd1ca259efd38d560ac3c44e6353af81a9b72e3/"
#         DESTINATION
#         "${CMAKE_CURRENT_SOURCE_DIR}/imgui/lib/")
# endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core
                 ${CMAKE_CURRENT_BINARY_DIR}/core-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gl
                 ${CMAKE_CURRENT_BINARY_DIR}/gl-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gfx
                 ${CMAKE_CURRENT_BINARY_DIR}/gfx-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui
                 ${CMAKE_CURRENT_BINARY_DIR}/imgui-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/input
                 ${CMAKE_CURRENT_BINARY_DIR}/input-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mix
                 ${CMAKE_CURRENT_BINARY_DIR}/mix-output)
