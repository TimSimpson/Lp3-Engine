# *********************************************************************
# LP3_CORE
#       Handles very rudimentary, basic stuff all the other libs need.
# *********************************************************************
project(lp3_core CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(Boost REQUIRED)
find_package(sdl2 2.0.7)

add_library(core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/le.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/assert.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/casts.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sdl.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/Exception.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/exit.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/utils.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/assert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ErrOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ErrOut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/media.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/WCharToCharConverter.cpp
)
set_target_properties(core PROPERTIES OUTPUT_NAME "Lp3_Core")

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

# Note: GSL is assumed and obtained from cget.
target_link_libraries(core PUBLIC Boost::boost sdl2::sdl2 sdl2::image)

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    target_sources(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/js/platform.cpp)
else()
    target_sources(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pc/platform.cpp)
endif()



# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS core EXPORT Lp3_Core-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::core ALIAS core)
install(EXPORT Lp3_Core-targets
    FILE Lp3_Core-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Core
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Core-config.cmake" "
include(CMakeFindDependencyMacro)
find_dependency(Boost)
find_dependency(sdl2)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Core-targets.cmake\")
")

write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Core-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Core-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Core-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Core
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

get_directory_property(is_child PARENT_DIRECTORY)
if (NOT is_child)
    set(LP3_CORE_FULL True)
endif()

if (LP3_CORE_FULL)
    add_executable(ExitDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/ExitDemo.cpp)
    target_link_libraries(ExitDemo core)

    add_executable(LogDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/LogDemo.cpp)
    target_link_libraries(LogDemo core)

    add_executable(MainDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/MainDemo.cpp)
    target_link_libraries(MainDemo core)

    add_executable(assert_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/assert_tests.cpp)
    target_link_libraries(assert_tests core)
    add_test(assert_tests assert_tests)

    add_executable(media_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/media_tests.cpp)
    target_link_libraries(media_tests core)
    add_test(media_tests media_tests)

    add_executable(Sdl2Simple ${CMAKE_CURRENT_SOURCE_DIR}/demos/Sdl2Simple.cpp)
    target_link_libraries(Sdl2Simple core)

    add_executable(MultipleWindows ${CMAKE_CURRENT_SOURCE_DIR}/demos/MultipleWindows.cpp)
    target_link_libraries(Sdl2Simple core)
    target_link_libraries(MultipleWindows core )

    if (LP3_SETUP_FOLDERS)
        set_target_properties (core PROPERTIES FOLDER core)
        set_target_properties (ExitDemo PROPERTIES FOLDER core)
        set_target_properties (LogDemo PROPERTIES FOLDER core)
        set_target_properties (MainDemo PROPERTIES FOLDER core)
        set_target_properties (assert_tests PROPERTIES FOLDER core)
        set_target_properties (media_tests PROPERTIES FOLDER core)
        set_target_properties (Sdl2Simple PROPERTIES FOLDER core)
        set_target_properties (MultipleWindows PROPERTIES FOLDER core)
    endif()
endif()

