cmake_minimum_required(VERSION 3.14)
project(action_planner)

ADD_CUSTOM_TARGET(actplnbindir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
    ${ROOT_BIN_DIR}/action_planner)

ADD_CUSTOM_TARGET(virbotplaningdir ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${ROOT_DATA_DIR}/ViRBot_Planning_Sockets
    ${ROOT_BIN_DIR}/action_planner/ViRBot_Planning_Sockets)
