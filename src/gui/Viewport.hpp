#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>


namespace ImNode = ax::NodeEditor;


class Viewport
{
public:

    Viewport();
    Viewport(const Viewport&) = delete;
    Viewport(Viewport&&) = delete;

    ~Viewport();

    void draw(bool* p_open);


private:

    ImNode::EditorContext* m_editorContext;

};
