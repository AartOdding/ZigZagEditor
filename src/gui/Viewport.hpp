#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>

#include <ZigZag/BaseOperator.hpp>

#include "app/ApplicationState.hpp"

namespace ImNode = ax::NodeEditor;



class Viewport
{
public:

    Viewport(std::string_view windowName, ApplicationState* appState);
    Viewport(const Viewport&) = delete;
    Viewport(Viewport&&) = delete;

    ~Viewport();

    void setScope(ZigZag::BaseOperator* scope);

    void draw(bool* open);


private:

    std::string m_windowName;

    ApplicationState* m_appState;

    ZigZag::BaseOperator* m_scope;
    
    ImNode::EditorContext* m_editorContext;

};
