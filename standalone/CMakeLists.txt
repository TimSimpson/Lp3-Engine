# This tests that the project will build correctly as a subproject.
# It's used only for CI; in your own project, ignore this and add Lp3-Core as
# a subproject (nested directory) using Git submodules or something else.
project(Lp3-Core-Parent CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)



if(MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compile_options(/WX)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GCC")
    add_compile_options(-std=c++14)
endif()



###############################################################################
#  Third Party libraries
###############################################################################

# If you are using this as a subproject, your super project CMakeLists.txt
# must define `boost` and `gsl` as interface import libraries as seen
# here.

# Otherwise, this will try to find Boost using CMake's built in methods
# and will download the GSL.
find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}")

# Microsoft Guidelines Support Library
include(ExternalProject)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/gsl/include")
ExternalProject_Add(
    GslProject
    GIT_REPOSITORY "https://github.com/Microsoft/GSL.git"
    GIT_TAG "3819df6e378ffccf0e29465afe99c3b324c2aa70"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/gsl"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    INSTALL_DIR ${PROJECT_BINARY_DIR}
)
add_library(gsl INTERFACE IMPORTED)
add_dependencies(gsl GslProject)
set_property(TARGET gsl PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/gsl/include")

###############################################################################
#  Subprojects
###############################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/output)

