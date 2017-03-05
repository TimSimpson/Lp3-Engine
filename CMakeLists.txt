# *********************************************************************
# LP3_CORE
#       Handles very rudimentary, basic stuff all the other libs need.
# *********************************************************************
project(lp3_core CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/le.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/assert.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/test.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/test_asserts.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/Clock.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/coroutine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/Exception.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/media.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/core/utils.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/ErrOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ErrOut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/media.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_asserts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/WCharToCharConverter.cpp)

target_include_directories(Lp3_Core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Core boost gsl)

install(TARGETS Lp3_Core EXPORT Lp3_Core_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Core_Config DESTINATION share/Lp3_Core/cmake)

export(TARGETS Lp3_Core FILE Lp3_Core_Config.cmake)


if (WIN32)
    target_sources(Lp3_Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/Clock.cpp)
    target_sources(Lp3_Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/platform.cpp)
endif()

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Emscripten")
    target_sources(Lp3_Core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/js/platform.cpp)
endif()

add_executable(AssertDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/AssertDemo.cpp)
target_link_libraries(AssertDemo Lp3_Core ${LP3_LIBS})

add_executable(LogDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/LogDemo.cpp)
target_link_libraries(LogDemo Lp3_Core ${LP3_LIBS})

add_executable(MainDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/MainDemo.cpp)
target_link_libraries(MainDemo Lp3_Core ${LP3_LIBS})

if (WIN32)
    add_executable(MultipleWindows ${CMAKE_CURRENT_SOURCE_DIR}/demos/MultipleWindows.cpp)
    target_link_libraries(MultipleWindows Lp3_Core ${LP3_LIBS})
endif()

add_executable(media_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/media_tests.cpp)
target_link_libraries(media_tests Lp3_Core ${LP3_LIBS})
add_test(media_tests media_tests)

add_executable(CoroutineTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/coroutine_tests.cpp)
target_link_libraries(CoroutineTests Lp3_Core ${LP3_LIBS})
add_test(CoroutineTests CoroutineTests)
