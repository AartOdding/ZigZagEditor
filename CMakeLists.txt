cmake_minimum_required(VERSION 3.12)

project(ZigZagEditorNative VERSION 0.1.0)

# To compile first creare a "build" directory in this folder.
# "mkdir build"

# Enter the build directory
# "cd build"

# Run cmake:
# "cmake .."

# To make a debug build:
# "cmake --build ."

# To make a release build:
# "cmake --build . --config Release"

# The generated binaries will be placed in the "build_output" folder

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build_output/Debug" CACHE INTERNAL "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build_output/Release" CACHE INTERNAL "" FORCE)


include("${CMAKE_CURRENT_LIST_DIR}/src/_src_files.cmake")

add_library(ZigZagEditorNative SHARED ${src_files})
target_include_directories(ZigZagEditorNative PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(ZigZagEditorNative PRIVATE ZIGZAG_IS_IMPLEMENTATION)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX Sources FILES ${src_files})


# Also build the native host
add_subdirectory(debug_host)


# FREETYPE2
add_subdirectory(external/freetype2)
target_link_libraries(ZigZagEditorNative freetype)

# GLAD
add_subdirectory(external/glad)
target_link_libraries(ZigZagEditorNative glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
target_link_libraries(ZigZagEditorNative glfw)

# GLM
target_include_directories(ZigZagEditorNative PUBLIC external/glm)

# IMGUI
add_subdirectory(external/imgui)
target_link_libraries(ZigZagEditorNative imgui)

# IMGUI-COLOR_TEXT_EDIT
add_subdirectory(external/ImGuiColorTextEdit)
target_link_libraries(ZigZagEditorNative ImGuiColorTextEdit)

# IMGUI-NODE-EDITOR
add_subdirectory(external/imgui-node-editor)
target_link_libraries(ZigZagEditorNative imgui_node_editor)

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")
add_subdirectory(external/json)
target_link_libraries(ZigZagEditorNative nlohmann_json)

# type_conversion_iterator
add_subdirectory(external/type_conversion_iterator)
target_link_libraries(ZigZagEditorNative type_conversion_iterator)

# ZIGZAG CORE
add_subdirectory(external/ZigZagCore)
target_link_libraries(ZigZagEditorNative ZigZagCore)
