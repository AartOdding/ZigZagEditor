cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(ZigZag VERSION 0.1.0)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-node-editor/Support/CMakeModules)

SET(HEADERS
    src/Application.hpp
    src/types.hpp

    src/app/Action.hpp
    src/app/AddObjectCommand.hpp
    src/app/Command.hpp
    src/app/CommandStack.hpp
    src/app/ConnectCommand.hpp
    src/app/DisconnectCommand.hpp
    src/app/RemoveObjectCommand.hpp
    src/app/RenameObjectCommand.hpp
    src/app/WindowActions.hpp

    src/gui/ApplicationStyle.hpp
    src/gui/HistoryView.hpp
    src/gui/MainMenu.hpp
    src/gui/ObjectInspector.hpp
    src/gui/RenderOrderWindow.hpp
    src/gui/Viewport.hpp
)

SET(SOURCES
    src/Application.cpp
    src/main.cpp

    src/app/Action.cpp
    src/app/AddObjectCommand.cpp
    src/app/CommandStack.cpp
    src/app/ConnectCommand.cpp
    src/app/DisconnectCommand.cpp
    src/app/RemoveObjectCommand.cpp
    src/app/RenameObjectCommand.cpp

    src/gui/ApplicationStyle.cpp
    src/gui/HistoryView.cpp
    src/gui/MainMenu.cpp
    src/gui/ObjectInspector.cpp
    src/gui/RenderOrderWindow.cpp
    src/gui/Viewport.cpp
)

add_executable(ZigZag ${HEADERS} ${SOURCES})

target_include_directories(ZigZag PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


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

# IMGUI-NODE-EDITOR
add_subdirectory(external/imgui-node-editor/NodeEditor)
target_link_libraries(ZigZag imgui_node_editor)

# ZIGZAG CORE
add_subdirectory(external/ZigZagCore)
target_link_libraries(ZigZag ZigZagCore)

# RESOURCES
target_include_directories(ZigZag PRIVATE resources)
# resource files have been generated using xxd. See:
# https://stackoverflow.com/questions/45802537/how-to-embed-font-and-other-image-files-when-using-graphic-libraries-like-sfml


