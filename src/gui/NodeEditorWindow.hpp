#pragma once

#include <imgui.h>
#include <imgui_node_editor.h>

#include <ZigZag/BaseOperator.hpp>

#include <app/ApplicationState.hpp>
#include <gui/Window.hpp>
#include <gui/popups/OperatorSelectionPopup.hpp>
#include <object/ObjectType.hpp>
#include <object/ZObject.hpp>

using namespace ax;



class NodeEditorWindow : public Window
{
public:

    NodeEditorWindow(std::string_view windowName, bool open = true);
    NodeEditorWindow(const NodeEditorWindow&) = delete;
    NodeEditorWindow(NodeEditorWindow&&) = delete;

    ~NodeEditorWindow();

    void setScope(Identifier<ZObject> scope);

protected:

    void draw() override;


private:

    Identifier<ZObject> m_scope;

    OperatorSelectionPopup m_operatorSelectionPopup;
    
    NodeEditor::EditorContext* m_editorContext;

};
