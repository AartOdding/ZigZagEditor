cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(ZigZag VERSION 0.1.0)


include("${CMAKE_CURRENT_LIST_DIR}/src/_src_files.cmake")

add_executable(ZigZag ${src_files})

target_include_directories(ZigZag PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# FREETYPE2
add_subdirectory(external/freetype2)
target_link_libraries(ZigZag freetype)

# GLAD
add_subdirectory(external/glad)
target_link_libraries(ZigZag glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
target_link_libraries(ZigZag glfw)

# GLM
target_include_directories(ZigZag PUBLIC external/glm)

# IMGUI
add_subdirectory(external/imgui)
target_link_libraries(ZigZag imgui)

# IMGUI-COLOR_TEXT_EDIT
add_subdirectory(external/ImGuiColorTextEdit)
target_link_libraries(ZigZag ImGuiColorTextEdit)

# IMGUI-NODE-EDITOR
add_subdirectory(external/imgui-node-editor)
target_link_libraries(ZigZag imgui_node_editor)

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")
add_subdirectory(external/json)
target_link_libraries(ZigZag nlohmann_json)

# ZIGZAG CORE
add_subdirectory(external/ZigZagCore)
target_link_libraries(ZigZag ZigZagCore)

# RESOURCES
target_include_directories(ZigZag PRIVATE resources)
# resource files have been generated using xxd. See:
# https://stackoverflow.com/questions/45802537/how-to-embed-font-and-other-image-files-when-using-graphic-libraries-like-sfml


