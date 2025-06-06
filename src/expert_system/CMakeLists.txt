cmake_minimum_required(VERSION 3.14)
project(expsys)

file(MAKE_DIRECTORY
  ${ROOT_BIN_DIR}/tcpCLIPS60
)

ADD_CUSTOM_TARGET(expsys ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}
    ${ROOT_BIN_DIR}/tcpCLIPS60/expert_system
  )
