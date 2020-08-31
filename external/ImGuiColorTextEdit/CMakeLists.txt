cmake_minimum_required(VERSION 3.0)

add_library(ImGuiColorTextEdit STATIC ImGuiColorTextEdit/TextEditor.cpp)

target_link_libraries(ImGuiColorTextEdit imgui)
target_include_directories(ImGuiColorTextEdit PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ImGuiColorTextEdit")
