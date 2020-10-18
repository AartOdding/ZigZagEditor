#include <Application.hpp>
#include <gui/NodeEditorWindow.hpp>
#include <app/command/AddObjectCommand.hpp>
#include <app/command/ConnectCommand.hpp>
#include <app/command/DisconnectCommand.hpp>
#include <app/command/RemoveObjectCommand.hpp>
#include <interop/ObjectInterop.hpp>

#include <iostream>

NodeEditorWindow::NodeEditorWindow(std::string_view windowName, bool open)
    : Window(windowName, open)
{
    NodeEditor::Config cfg;
    cfg.SettingsFile = nullptr;
    m_editorContext = NodeEditor::CreateEditor(&cfg);
}


NodeEditorWindow::~NodeEditorWindow()
{
    NodeEditor::DestroyEditor(m_editorContext);
}


void NodeEditorWindow::setScope(Identifier<ZObject> scope)
{
    m_scope = scope;
}

void NodeEditorWindow::draw()
{
    ImGui::PushID(this);

    NodeEditor::SetCurrentEditor(m_editorContext);
    NodeEditor::Begin(getTitle().c_str());

    auto currentObject = IdentityMap<ZObject>::get(m_scope);

    if (currentObject)
    {
        for (auto child : currentObject->getChildren())
        {
            NodeEditor::BeginNode(NodeEditor::NodeId(child));

            auto localPos = ImGui::GetCursorPos();
            ImGui::Dummy({ 100, 100 });
            ImGui::SetCursorPos({ localPos.x + 10, localPos.y + 10 });
            ImGui::Text(child->getName().c_str());

            int inputCount = 0;
            int outputCount = 0;

            // while iterating the children make a list of what needs to be connected
            for (auto grandChild : child->getChildren())
            {
                if (grandChild->getNodeCategory() == ObjectTypeCategory::OperatorInput)
                {
                    int y = 30 + inputCount++ * 30;
                    ImGui::SetCursorPos({ localPos.x, localPos.y + y });
                    NodeEditor::BeginPin(NodeEditor::PinId(grandChild), NodeEditor::PinKind::Input);
                    ImGui::Dummy({ 20, 20 });
                    NodeEditor::EndPin();
                }
                else if (grandChild->getNodeCategory() == ObjectTypeCategory::OperatorOutput)
                {
                    int y = 30 + outputCount++ * 30;
                    ImGui::SetCursorPos({ localPos.x + 80, localPos.y + y });
                    NodeEditor::BeginPin(NodeEditor::PinId(grandChild), NodeEditor::PinKind::Output);
                    ImGui::Dummy({ 20, 20 });
                    NodeEditor::EndPin();
                }
            }
            NodeEditor::EndNode();
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
                        //m_appState->commandStack.push<ConnectDataCommand>(startSource, finishInput);
                    }
                }
                else if (auto startInput = dynamic_cast<ZigZag::BaseDataInput*>(start))
                {
                    if (auto finishSource = dynamic_cast<ZigZag::BaseDataSource*>(finish))
                    {
                        //m_appState->commandStack.push<ConnectDataCommand>(finishSource, startInput);
                    }
                }
            }

        }
    }
    NodeEditor::EndCreate();

    if (NodeEditor::BeginDelete())
    {
        // clear deleted links

        NodeEditor::LinkId linkId;
        NodeEditor::PinId startId, finishId;

        if (NodeEditor::QueryDeletedLink(&linkId))
        {
            if (NodeEditor::AcceptDeletedItem())
            {
                auto input = linkId.AsPointer<ZigZag::BaseDataInput>();
                //m_appState->commandStack.push<DisconnectDataCommand>(input->getConnectedOutput(), input);
            }
        }

        // clear deleted nodes

        NodeEditor::NodeId nodeId;
        
        if (NodeEditor::QueryDeletedNode(&nodeId))
        {
            if (NodeEditor::AcceptDeletedItem())
            {
                auto ptr = nodeId.AsPointer<const ZObject>();
                removeObject(ptr->getIdentifier());
            }
        }
    }
    NodeEditor::EndDelete();

    bool openPopup = NodeEditor::IsBackgroundDoubleClicked();


    NodeEditor::End();

    ImGui::PopID();

    // Update the operator selection popup

    if (openPopup)
    {
        auto sz = ImGui::GetWindowSize();
        m_operatorSelectionPopup.open({ 0.5f * sz.x, 0.5f * sz.y });
    }

    m_operatorSelectionPopup.update();

    if (m_operatorSelectionPopup.getConfirmedOperator())
    {
        auto typeID = m_operatorSelectionPopup.getConfirmedOperator();
        auto root = Application::getGlobalInstance()->getRootObject();
        if (root)
        {
            auto appState = Application::getGlobalInstance()->getAppState();
            appState->commandStack.push<AddObjectCommand>(typeID, root->getIdentifier());
            //addObject(m_operatorSelectionPopup.getConfirmedOperator(), root->getIdentifier());
        }
    }

}
