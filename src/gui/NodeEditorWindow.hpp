#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>

#include <ZigZag/BaseOperator.hpp>

#include "app/ApplicationState.hpp"
#include "gui/Window.hpp"

using namespace ax;



class NodeEditorWindow : public Window
{
public:

    NodeEditorWindow(std::string_view windowName, ApplicationState* appState);
    NodeEditorWindow(const NodeEditorWindow&) = delete;
    NodeEditorWindow(NodeEditorWindow&&) = delete;

    ~NodeEditorWindow();

    void setScope(ZigZag::BaseOperator* scope);

protected:

    void draw() override;


private:

    ApplicationState* m_appState;

    ZigZag::BaseOperator* m_scope;
    
    NodeEditor::EditorContext* m_editorContext;

};
