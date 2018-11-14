# *********************************************************************
# LP3_SIMS
#       Simulation code - handles processes, events, coroutines, etc.
# *********************************************************************
project(lp3_sims CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
include(CMakePackageConfigHelpers)

find_package(Lp3_Core 1.0.1)
find_package(catch2)

add_library(sims
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/Clock.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/coroutine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/dispatcher.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/EventSimulator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lp3/sims/qroutines.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventSimulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qroutines.cpp)
set_target_properties(sims PROPERTIES OUTPUT_NAME "Lp3_Sims")

target_include_directories(sims
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(sims PUBLIC lp3::core)

# *********************************************************************
# Package / Install Stuff
# *********************************************************************

install(DIRECTORY include/ DESTINATION include)

install(TARGETS sims EXPORT Lp3_Sims-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

add_library(lp3::sims ALIAS sims)
install(EXPORT Lp3_Sims-targets
    FILE Lp3_Sims-targets.cmake
    NAMESPACE lp3::
    DESTINATION lib/cmake/Lp3_Sims
)

file(WRITE "${PROJECT_BINARY_DIR}/Lp3_Sims-config.cmake" "
include(CMakeFindDependencyMacro)
find_dependency(Lp3_Core)
include(\"\${CMAKE_CURRENT_LIST_DIR}/Lp3_Sims-targets.cmake\")
")

write_basic_package_version_file("${PROJECT_BINARY_DIR}/Lp3_Sims-config-version.cmake"
    VERSION 1.0.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/Lp3_Sims-config.cmake"
    "${PROJECT_BINARY_DIR}/Lp3_Sims-config-version.cmake"
    DESTINATION lib/cmake/Lp3_Sims
)

# *********************************************************************
# Tests and Drivers / Demos
# *********************************************************************

if (LP3_SIMS_FULL)
    add_library(tests-main STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests-main.cpp)
    target_link_libraries(tests-main catch2::catch2)

    add_executable(CoroutineTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/coroutine_tests.cpp)
    target_link_libraries(CoroutineTests sims tests-main)
    add_test(CoroutineTests CoroutineTests)

    add_executable(dispatcher_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/dispatcher_tests.cpp)
    target_link_libraries(dispatcher_tests sims tests-main)
    add_test(dispatcher_tests dispatcher_tests)

    add_executable(qroutines_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/qroutines_tests.cpp)
    target_link_libraries(qroutines_tests sims tests-main)
    add_test(qroutines_tests qroutines_tests)

    add_executable(EventSimulatorTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/EventSimulatorTests.cpp)
    target_link_libraries(EventSimulatorTests sims tests-main)
    add_test(EventSimulatorTests EventSimulatorTests)

    add_executable(TimersDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/TimersDemo.cpp)
    target_link_libraries(TimersDemo sims)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (sims PROPERTIES FOLDER sims)
        set_target_properties (tests-main PROPERTIES FOLDER sims)
        set_target_properties (CoroutineTests PROPERTIES FOLDER sims)
        set_target_properties (dispatcher_tests PROPERTIES FOLDER sims)
        set_target_properties (qroutines_tests PROPERTIES FOLDER sims)
        set_target_properties (EventSimulatorTests PROPERTIES FOLDER sims)
        set_target_properties (TimersDemo PROPERTIES FOLDER sims)
    endif()
endif()
