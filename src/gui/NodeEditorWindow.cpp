#include "gui/NodeEditorWindow.hpp"
#include "app/command/ConnectCommand.hpp"
#include "app/command/DisconnectCommand.hpp"
#include "app/command/RemoveObjectCommand.hpp"

#include <iostream>

NodeEditorWindow::NodeEditorWindow(std::string_view windowName, ApplicationState* appState)
    : Window(windowName)
    ,  m_appState(appState)
    ,  m_scope(&appState->rootOperator)
{
    NodeEditor::Config cfg;
    cfg.SettingsFile = nullptr;
    m_editorContext = NodeEditor::CreateEditor(&cfg);
}


NodeEditorWindow::~NodeEditorWindow()
{
    NodeEditor::DestroyEditor(m_editorContext);
}


void NodeEditorWindow::setScope(ZigZag::BaseOperator* scope)
{
    m_scope = scope;
}

void NodeEditorWindow::draw()
{
    ImGui::PushID(this);

    NodeEditor::SetCurrentEditor(m_editorContext);
    NodeEditor::Begin(getTitle().c_str());

    //ImNode::PushStyleVar(ImNode::StyleVar_NodePadding, {0, 0, 0, 0});
    //ImNode::PushStyleVar(ImNode::StyleVar_NodeRounding, 3);
    //ImNode::PushStyleVar(ImNode::StyleVar_NodeBorderWidth, 0);
    //ImNode::PushStyleVar(ImNode::StyleVar_HoveredNodeBorderWidth, 0);
    //ImNode::PushStyleVar(ImNode::StyleVar_SelectedNodeBorderWidth, 0);

    //ImNode::PushStyleColor(ImNode::StyleColor_NodeBg, { 1, 1, 1, 0 });

    if (m_scope)
    {
        for (auto op : m_scope->getChildOperators())
        {
            NodeEditor::BeginNode(NodeEditor::NodeId(static_cast<ZigZag::Object*>(op)));
            auto localPos = ImGui::GetCursorPos();
            ImGui::Dummy({ 100, 100 });
            ImGui::SetCursorPos({ localPos.x + 10, localPos.y + 10 });
            ImGui::Text(op->getName().c_str());

            auto& inputs = op->getDataInputs();
            auto& outputs = op->getDataSources();

            auto max = std::max(inputs.size(), outputs.size());

            for (int i = 0; i < inputs.size(); ++i)
            {
                int y = 30 + i * 30;
                ImGui::SetCursorPos({ localPos.x, localPos.y + y });
                NodeEditor::BeginPin(
                    NodeEditor::PinId(static_cast<ZigZag::Object*>(inputs[i])),
                    NodeEditor::PinKind::Input);
                ImGui::Dummy({ 20, 20 });
                NodeEditor::EndPin();
            }
            for (int i = 0; i < outputs.size(); ++i)
            {
                int y = 30 + i * 30;
                ImGui::SetCursorPos({ localPos.x + 80, localPos.y + y });
                NodeEditor::BeginPin(
                    NodeEditor::PinId(static_cast<ZigZag::Object*>(outputs[i])),
                    NodeEditor::PinKind::Output);
                ImGui::Dummy({ 20, 20 });
                NodeEditor::EndPin();
            }
            NodeEditor::EndNode();
        }
        for (auto op : m_scope->getChildOperators())
        {
            for (auto output : op->getDataSources())
            {
                for (auto input : output->getConnectedInputs())
                {
                    // use input for link id because input can only have one conection.
                    NodeEditor::Link(
                        NodeEditor::LinkId(static_cast<ZigZag::Object*>(input)),
                        NodeEditor::PinId(static_cast<ZigZag::Object*>(output)),
                        NodeEditor::PinId(static_cast<ZigZag::Object*>(input)));
                }
            }
        }
    }

    if (NodeEditor::BeginCreate())
    {
        NodeEditor::PinId startId, finishId;

        // The order of the output and input in QueryNewLink is undefined. Therefore we need to test
        // if there is a output and an input.
        if (NodeEditor::QueryNewLink(&startId, &finishId))
        {
            // AcceptNewItem returns true if the mouse is released
            if (NodeEditor::AcceptNewItem())
            {
                auto start = startId.AsPointer<ZigZag::Object>();
                auto finish = finishId.AsPointer<ZigZag::Object>();

                if (auto startSource = dynamic_cast<ZigZag::BaseDataSource*>(start))
                {
                    if (auto finishInput = dynamic_cast<ZigZag::BaseDataInput*>(finish))
                    {
                        m_appState->commandStack.push<ConnectDataCommand>(startSource, finishInput);
                    }
                }
                else if (auto startInput = dynamic_cast<ZigZag::BaseDataInput*>(start))
                {
                    if (auto finishSource = dynamic_cast<ZigZag::BaseDataSource*>(finish))
                    {
                        m_appState->commandStack.push<ConnectDataCommand>(finishSource, startInput);
                    }
                }
            }

        }
    }
    NodeEditor::EndCreate();

    if (NodeEditor::BeginDelete())
    {
        NodeEditor::LinkId linkId;
        NodeEditor::PinId startId, finishId;

        if (NodeEditor::QueryDeletedLink(&linkId))
        {
            if (NodeEditor::AcceptDeletedItem())
            {
                auto input = linkId.AsPointer<ZigZag::BaseDataInput>();
                m_appState->commandStack.push<DisconnectDataCommand>(input->getConnectedOutput(), input);
            }
        }
    }
    NodeEditor::EndDelete();

    if (NodeEditor::BeginDelete())
    {
        NodeEditor::NodeId nodeId;
        
        if (NodeEditor::QueryDeletedNode(&nodeId))
        {
            if (NodeEditor::AcceptDeletedItem())
            {
                m_appState->commandStack.push<RemoveObjectCommand>(nodeId.AsPointer<ZigZag::BaseOperator>());
            }
        }
    }
    NodeEditor::EndDelete();


    //ImNode::PopStyleColor(1);
    //ImNode::PopStyleVar(5);
    NodeEditor::End();

    //ImGui::PopStyleVar(1);
    ImGui::PopID();
}
