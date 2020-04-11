cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(ZigZag VERSION 0.1.0)



add_executable(ZigZag
    src/main.cpp
    #external/imgui/imgui/examples/example_glfw_opengl3/main.cpp
)

target_include_directories(ZigZag PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


# GLAD
add_subdirectory(external/glad)
target_link_libraries(ZigZag glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
target_link_libraries(ZigZag glfw)

# IMGUI
add_subdirectory(external/imgui)
target_link_libraries(ZigZag imgui)

# ZIGZAG CORE
add_subdirectory(external/ZigZagCore)
target_link_libraries(ZigZag ZigZagCore)

