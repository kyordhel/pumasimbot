cmake_minimum_required(VERSION 3.14)
project(gui)

ADD_CUSTOM_TARGET(guibindir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
    ${ROOT_BIN_DIR}/gui)

ADD_CUSTOM_TARGET(pumasimbot ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}/pumasimbot.py
    ${ROOT_BIN_DIR}/gui/pumasimbot.py)

ADD_CUSTOM_TARGET(initialbh ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}/initial_behaviors.py
    ${ROOT_BIN_DIR}/gui/initial_behaviors.py)
