cmake_minimum_required(VERSION 3.0)


set(command_files
    "${CMAKE_CURRENT_LIST_DIR}/Command.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/CommandStack.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/CommandStack.hpp"

    "${CMAKE_CURRENT_LIST_DIR}/AddObjectCommand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/AddObjectCommand.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/RemoveObjectCommand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RemoveObjectCommand.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/RenameObjectCommand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RenameObjectCommand.hpp"

    "${CMAKE_CURRENT_LIST_DIR}/ConnectCommand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ConnectCommand.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/DisconnectCommand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DisconnectCommand.hpp"
)
