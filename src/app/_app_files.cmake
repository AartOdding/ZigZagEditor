cmake_minimum_required(VERSION 3.0)

include("${CMAKE_CURRENT_LIST_DIR}/command/_command_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/style/_style_files.cmake")


set(app_files
    "${CMAKE_CURRENT_LIST_DIR}/_app_files.cmake"

    "${command_files}"
    "${style_files}"

    "${CMAKE_CURRENT_LIST_DIR}/ApplicationState.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/Directories.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Directories.hpp"
)
