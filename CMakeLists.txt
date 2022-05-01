# This parent project installs all the subprojects, which makes this repository
# compatable with CGet.

# See standalone/CMakeLists.txt for an example of using them in integrated
# builds.
project(lp3_engine-Install CXX)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/lib/imgui.h")
    file(DOWNLOAD
            "https://github.com/ocornut/imgui/archive/1cd1ca259efd38d560ac3c44e6353af81a9b72e3.tar.gz"
            "${PROJECT_BINARY_DIR}/imgui.tar.gz"
             SHOW_PROGRESS
             EXPECTED_MD5 "1c0d601fa39235a4e3611c206403a49a")
    execute_process(
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${PROJECT_BINARY_DIR}/imgui.tar.gz"
    )
    file(COPY
        "${PROJECT_BINARY_DIR}/imgui-1cd1ca259efd38d560ac3c44e6353af81a9b72e3/"
        DESTINATION
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/lib/")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core
                 ${CMAKE_CURRENT_BINARY_DIR}/core-output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sims
                 ${CMAKE_CURRENT_BINARY_DIR}/sims-output)

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
