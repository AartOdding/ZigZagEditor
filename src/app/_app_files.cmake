cmake_minimum_required(VERSION 3.0)

include("${CMAKE_CURRENT_LIST_DIR}/command/_command_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/style/_style_files.cmake")


set(app_files
    "${command_files}"
    "${style_files}"

    "${CMAKE_CURRENT_LIST_DIR}/Action.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Action.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/ApplicationState.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/WindowActions.hpp"
)
