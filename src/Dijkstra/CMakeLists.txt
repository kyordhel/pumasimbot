cmake_minimum_required(VERSION 3.6)
project(Dijkstra)

## Compile as C++14
add_compile_options(-std=c++14)
find_package(Boost REQUIRED COMPONENTS thread filesystem)

set(EXECUTABLE_OUTPUT_PATH "${ROOT_BIN_DIR}/Dijkstra")

add_executable(Dijkstra
  Dijkstra.cpp
)

# Linker dependencies
target_link_libraries(Dijkstra
  Boost::filesystem
  Boost::thread
  ${BOOST_LIBRARIES}
)
