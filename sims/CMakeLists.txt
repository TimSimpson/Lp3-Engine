# *********************************************************************
# LP3_SIMS
#       Simulation code - handles processes, events, coroutines, etc.
# *********************************************************************
project(lp3_sims CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(Lp3_Sims
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

target_include_directories(Lp3_Sims
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

target_link_libraries(Lp3_Sims Lp3_Core)

install(TARGETS Lp3_Sims EXPORT Lp3_Sims_Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

install(EXPORT Lp3_Sims_Config DESTINATION share/Lp3_Sims/cmake)

export(TARGETS Lp3_Sims FILE Lp3_Sims_Config.cmake)

if (LP3_SIMS_FULL)
    add_library(tests-main STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests-main.cpp)
    target_link_libraries(tests-main lp3_deps::catch)

    add_executable(CoroutineTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/coroutine_tests.cpp)
    target_link_libraries(CoroutineTests Lp3_Sims tests-main)
    add_test(CoroutineTests CoroutineTests)

    add_executable(dispatcher_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/dispatcher_tests.cpp)
    target_link_libraries(dispatcher_tests Lp3_Sims tests-main)
    add_test(dispatcher_tests dispatcher_tests)

    add_executable(qroutines_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/qroutines_tests.cpp)
    target_link_libraries(qroutines_tests Lp3_Sims tests-main)
    add_test(qroutines_tests qroutines_tests)

    add_executable(EventSimulatorTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/EventSimulatorTests.cpp)
    target_link_libraries(EventSimulatorTests Lp3_Sims tests-main)
    add_test(EventSimulatorTests EventSimulatorTests)

    add_executable(TimersDemo ${CMAKE_CURRENT_SOURCE_DIR}/demos/TimersDemo.cpp)
    target_link_libraries(TimersDemo Lp3_Sims)

    if (LP3_SETUP_FOLDERS)
        set_target_properties (Lp3_Sims PROPERTIES FOLDER sims)
        set_target_properties (tests-main PROPERTIES FOLDER sims)
        set_target_properties (CoroutineTests PROPERTIES FOLDER sims)
        set_target_properties (dispatcher_tests PROPERTIES FOLDER sims)
        set_target_properties (qroutines_tests PROPERTIES FOLDER sims)
        set_target_properties (EventSimulatorTests PROPERTIES FOLDER sims)
        set_target_properties (TimersDemo PROPERTIES FOLDER sims)
    endif()
endif()
