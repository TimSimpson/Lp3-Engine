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

###############################################################################
# Adds SDL2 libraries
# These names are based on the following package I used to use with cget:
# https://github.com/TimSimpson/download-sdl2/blob/master/sdl2-config.cmake
#
# These days it seems the standard CMake targets come from vcpkg. Maybe one
# day I'll change them.
###############################################################################

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


set(LP3_SETUP_FOLDERS TRUE)  # Organizes VS folders


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
