cmake_minimum_required(VERSION 2.8)

set( CMAKE_CXX_STANDARD 17 )

project(ZigZag)


add_executable(ZigZag
    src/main.cpp
)

#${CMAKE_CURRENT_SOURCE_DIR}
target_include_directories(ZigZag PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
