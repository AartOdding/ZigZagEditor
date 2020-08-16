cmake_minimum_required(VERSION 3.0)

add_library(imgui_node_editor STATIC
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/crude_json.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/crude_json.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_bezier_math.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_bezier_math.inl"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_canvas.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_canvas.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_extra_math.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_extra_math.inl"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_node_editor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_node_editor.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_node_editor_api.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_node_editor_internal.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor/imgui_node_editor_internal.inl"
)

target_link_libraries(imgui_node_editor imgui)
target_include_directories(imgui_node_editor PUBLIC "${CMAKE_CURRENT_LIST_DIR}/imgui-node-editor")
