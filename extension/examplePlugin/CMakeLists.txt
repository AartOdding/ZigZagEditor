cmake_minimum_required(VERSION 3.0)
project(examplePlugin)

add_library(examplePlugin SHARED examplePlugin.cpp)

target_include_directories(examplePlugin PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include")

