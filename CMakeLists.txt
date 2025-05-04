cmake_minimum_required(VERSION 3.6)
project(pumasimbot)

## Compile as C++14
add_compile_options(-std=c++14)
find_package(Boost REQUIRED COMPONENTS thread filesystem)

set(TCP_CLIPS60_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/tcpCLIPS60)
include(tcpCLIPS60.cmake)

set(ROOT_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(ROOT_DATA_DIR ${CMAKE_SOURCE_DIR}/data)

include_directories(
  ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(src/action_planner)
add_subdirectory(src/motion_planner)
add_subdirectory(src/Dijkstra)
add_subdirectory(src/gui)
