# *********************************************************************
# LP3_CORE
#       Handles very rudimentary, basic stuff all the other libs need.
# *********************************************************************
project(lp3_core CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Core
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


target_include_directories(Lp3_Core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Core lp3_deps::boost lp3_deps::gsl lp3_deps::sdl2 lp3_deps::sdl2_image)

install(TARGETS Lp3_Core EXPORT Lp3_Core_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Core_Config DESTINATION share/Lp3_Core/cmake)

export(TARGETS Lp3_Core FILE Lp3_Core_Config.cmake)


if ("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    target_sources(Lp3_Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/js/platform.cpp)
else()
    target_sources(Lp3_Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pc/platform.cpp)
endif()

if (LP3_CORE_FULL)
    add_executable(ExitDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/ExitDemo.cpp)
    target_link_libraries(ExitDemo Lp3_Core)

    add_executable(LogDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/LogDemo.cpp)
    target_link_libraries(LogDemo Lp3_Core)

    add_executable(MainDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/MainDemo.cpp)
    target_link_libraries(MainDemo Lp3_Core)

    add_executable(assert_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/assert_tests.cpp)
    target_link_libraries(assert_tests Lp3_Core lp3_deps::catch)
    add_test(assert_tests assert_tests)

    add_executable(media_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/media_tests.cpp)
    target_link_libraries(media_tests Lp3_Core lp3_deps::catch)
    add_test(media_tests media_tests)

    add_executable(Sdl2Simple ${CMAKE_CURRENT_SOURCE_DIR}/demos/Sdl2Simple.cpp)
    target_link_libraries(Sdl2Simple Lp3_Core)

    add_executable(MultipleWindows ${CMAKE_CURRENT_SOURCE_DIR}/demos/MultipleWindows.cpp)
    target_link_libraries(MultipleWindows Lp3_Core)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_Core PROPERTIES FOLDER core)
        set_target_properties (ExitDemo PROPERTIES FOLDER core)
        set_target_properties (LogDemo PROPERTIES FOLDER core)
        set_target_properties (MainDemo PROPERTIES FOLDER core)
        set_target_properties (assert_tests PROPERTIES FOLDER core)
        set_target_properties (media_tests PROPERTIES FOLDER core)
        set_target_properties (Sdl2Simple PROPERTIES FOLDER core)
        set_target_properties (MultipleWindows PROPERTIES FOLDER core)
    endif()
endif()
