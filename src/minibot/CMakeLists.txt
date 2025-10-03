cmake_minimum_required(VERSION 3.14)
project(libminibot)

## Compile as C++14
add_compile_options(-std=c++14)
find_package(Boost COMPONENTS thread filesystem REQUIRED)

set(EXECUTABLE_OUTPUT_PATH "${ROOT_BIN_DIR}/minibot")

add_library(minibot STATIC
  minibot.cpp
)

# Linker dependencies
target_link_libraries(minibot
  minibot-client
  Boost::thread
  Boost::filesystem
  ${BOOST_LIBRARIES}
)
