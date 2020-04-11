
add_library(imgui
    imgui/imgui.cpp 
    imgui/imgui_demo.cpp 
    imgui/imgui_draw.cpp 
    imgui/imgui_widgets.cpp 
    imgui/examples/imgui_impl_glfw.cpp 
    imgui/examples/imgui_impl_opengl3.cpp 
)

target_link_libraries(imgui glad)
target_link_libraries(imgui glfw)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples)
