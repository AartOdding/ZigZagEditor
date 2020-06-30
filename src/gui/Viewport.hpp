#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>

#include <ZigZag/BaseOperator.hpp>

namespace ImNode = ax::NodeEditor;


class Viewport
{
public:

    Viewport();
    Viewport(const Viewport&) = delete;
    Viewport(Viewport&&) = delete;

    ~Viewport();

    void setScope(ZigZag::BaseOperator* scope);

    void draw(bool* open);


private:

    ZigZag::BaseOperator* m_scope{ nullptr };
    
    ImNode::EditorContext* m_editorContext{ nullptr };

};
