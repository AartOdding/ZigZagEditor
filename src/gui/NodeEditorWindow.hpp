#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>

#include <ZigZag/BaseOperator.hpp>

#include <app/ApplicationState.hpp>
#include <gui/Window.hpp>
#include <gui/popups/OperatorSelectionPopup.hpp>
#include <object/Node.hpp>
#include <object/NodeTemplate.hpp>

using namespace ax;



class NodeEditorWindow : public Window
{
public:

    NodeEditorWindow(std::string_view windowName, bool open = true);
    NodeEditorWindow(const NodeEditorWindow&) = delete;
    NodeEditorWindow(NodeEditorWindow&&) = delete;

    ~NodeEditorWindow();

    void setScope(Identifier<Node> scope);

protected:

    void draw() override;


private:

    Identifier<Node> m_scope;

    OperatorSelectionPopup m_operatorSelectionPopup;
    
    NodeEditor::EditorContext* m_editorContext;

};
