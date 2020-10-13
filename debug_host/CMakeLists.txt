cmake_minimum_required(VERSION 3.12)

project(ZigZagEditorNativeHost VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)



add_executable(ZigZagEditorNativeHost src/main.cpp)

target_link_libraries(ZigZagEditorNativeHost PRIVATE ZigZagEditorNative)

