project(lp3_engine-Parent CXX)
cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 17)

find_package(lp3-engine)
find_package(lp3-main)

add_executable(ExitDemo WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/../core/demos/ExitDemo.cpp)
target_link_libraries(ExitDemo lp3::core lp3::main)

add_executable(TextureDemo WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/../gfx/demos/TextureDemo.cpp)
target_link_libraries(TextureDemo lp3::sims lp3::gfx lp3::main)

add_executable(pokey WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/../input/demos/pokey.cpp)
target_link_libraries(pokey lp3::gfx lp3::input lp3::main)

add_executable(mix_demo WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/../mix/demos/mix_demo.cpp)
target_link_libraries(mix_demo lp3::gfx lp3::input lp3::mix lp3::imgui lp3::main lp3::sims)
