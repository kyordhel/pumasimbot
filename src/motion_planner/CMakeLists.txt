cmake_minimum_required(VERSION 3.6)
project(motion_planner)

## Compile as C++14
add_compile_options(-std=c++14)
find_package(Boost REQUIRED COMPONENTS thread filesystem)

set(EXECUTABLE_OUTPUT_PATH "${ROOT_BIN_DIR}/motion_planner")

add_executable(GoTo_State_Machine
  GoTo_State_Machine.cpp
)

# Linker dependencies
target_link_libraries(GoTo_State_Machine
  Boost::filesystem
  Boost::thread
  ${BOOST_LIBRARIES}
)
