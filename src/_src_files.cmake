cmake_minimum_required(VERSION 3.0)

include("${CMAKE_CURRENT_LIST_DIR}/app/_app_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/gui/_gui_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/interop/_interop_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/object/_object_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/util/_util_files.cmake")


set(src_files
    "${app_files}"
    "${gui_files}"
    "${interop_files}"
    "${object_files}"
    "${util_files}"

    "${CMAKE_CURRENT_LIST_DIR}/Application.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Application.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Platform.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/types.hpp"
)
