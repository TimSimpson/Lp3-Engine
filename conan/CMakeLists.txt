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


# If testiing is required, turn on the "full" options for each subproject.
if(BUILD_TESTING AND LP3_ENGINE_Build_Tests)
    find_package(Catch2 REQUIRED)

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


add_library(engine INTERFACE)
target_link_libraries(engine PUBLIC INTERFACE lp3::core lp3::gl lp3::gfx lp3::imgui lp3::input lp3::mix lp3::main lp3::sims)

add_library(lp3::engine ALIAS engine)

# TODO: support traditional CMake package install, maybe?
