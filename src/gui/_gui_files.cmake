cmake_minimum_required(VERSION 3.0)

include("${CMAKE_CURRENT_LIST_DIR}/popups/_popup_files.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/widgets/_widget_files.cmake")


set(gui_files
    "${CMAKE_CURRENT_LIST_DIR}/_gui_files.cmake"

    "${popup_files}"
    "${widget_files}"

    "${CMAKE_CURRENT_LIST_DIR}/HierarchyWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/HierarchyWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/HistoryWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/HistoryWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/LuaEditorWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/LuaEditorWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/MainMenu.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/MainMenu.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/NodeEditorWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/NodeEditorWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/RenderOrderWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RenderOrderWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/StyleEditorWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/StyleEditorWindow.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/Window.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Window.hpp"
)
