cmake_minimum_required(VERSION 3.14)
project(action_planner)

file(MAKE_DIRECTORY
  ${ROOT_BIN_DIR}/action_planner
)

ADD_CUSTOM_TARGET(virbotplaningdir1 ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}/ViRBot_Planning_Sockets
    ${ROOT_BIN_DIR}/action_planner/ViRBot_Planning_Sockets)

ADD_CUSTOM_TARGET(virbotplaningdir2 ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}/ViRBot_Planning_Sockets
    ${ROOT_BIN_DIR}/tcpCLIPS60/expert_system/ViRBot_Planning_Sockets)