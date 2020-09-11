cmake_minimum_required(VERSION 3.0)
project(examplePlugin)

add_compile_definitions(ZIGZAG_PLUGIN)
add_library(examplePlugin SHARED examplePlugin.cpp)

target_include_directories(examplePlugin PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../external/ZigZagCore/include")
