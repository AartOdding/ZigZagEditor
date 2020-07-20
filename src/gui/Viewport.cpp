#include "Viewport.hpp"
#include "app/ConnectCommand.hpp"
#include "app/DisconnectCommand.hpp"
#include "app/RemoveObjectCommand.hpp"

#include <iostream>

Viewport::Viewport(std::string_view windowName, ApplicationState* appState)
    : m_windowName(windowName),
      m_appState(appState),
      m_scope(&appState->rootOperator)
{
    ImNode::Config cfg;
    cfg.SettingsFile = nullptr;
    m_editorContext = ImNode::CreateEditor(&cfg);
}


Viewport::~Viewport()
{
    ImNode::DestroyEditor(m_editorContext);
}


void Viewport::setScope(ZigZag::BaseOperator* scope)
{
    m_scope = scope;
}

void Viewport::draw(bool* open)
{
    ImGui::PushID(this);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin(m_windowName.c_str(), open);

    ImNode::SetCurrentEditor(m_editorContext);
    ImNode::Begin(m_windowName.c_str());

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
            ImNode::BeginNode(ImNode::NodeId(static_cast<ZigZag::Object*>(op)));
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
                ImNode::BeginPin(
                    ImNode::PinId(static_cast<ZigZag::Object*>(inputs[i])),
                    ImNode::PinKind::Input);
                ImGui::Dummy({ 20, 20 });
                ImNode::EndPin();
            }
            for (int i = 0; i < outputs.size(); ++i)
            {
                int y = 30 + i * 30;
                ImGui::SetCursorPos({ localPos.x + 80, localPos.y + y });
                ImNode::BeginPin(
                    ImNode::PinId(static_cast<ZigZag::Object*>(outputs[i])), 
                    ImNode::PinKind::Output);
                ImGui::Dummy({ 20, 20 });
                ImNode::EndPin();
            }
            ImNode::EndNode();
        }
        for (auto op : m_scope->getChildOperators())
        {
            for (auto output : op->getDataSources())
            {
                for (auto input : output->getConnectedInputs())
                {
                    // use input for link id because input can only have one conection.
                    ImNode::Link(
                        ImNode::LinkId(static_cast<ZigZag::Object*>(input)), 
                        ImNode::PinId(static_cast<ZigZag::Object*>(output)), 
                        ImNode::PinId(static_cast<ZigZag::Object*>(input)));
                }
            }
        }
    }

    if (ImNode::BeginCreate())
    {
        ImNode::PinId startId, finishId;

        // The order of the output and input in QueryNewLink is undefined. Therefore we need to test
        // if there is a output and an input.
        if (ImNode::QueryNewLink(&startId, &finishId))
        {
            // AcceptNewItem returns true if the mouse is released
            if (ImNode::AcceptNewItem())
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
    ImNode::EndCreate();

    if (ImNode::BeginDelete())
    {
        ImNode::LinkId linkId;
        ImNode::PinId startId, finishId;

        if (ImNode::QueryDeletedLink(&linkId))
        {
            if (ImNode::AcceptDeletedItem())
            {
                auto input = linkId.AsPointer<ZigZag::BaseDataInput>();
                m_appState->commandStack.push<DisconnectDataCommand>(input->getConnectedOutput(), input);
            }
        }
    }
    ImNode::EndDelete();

    if (ImNode::BeginDelete())
    {
        ImNode::NodeId nodeId;
        
        if (ImNode::QueryDeletedNode(&nodeId))
        {
            if (ImNode::AcceptDeletedItem())
            {
                m_appState->commandStack.push<RemoveObjectCommand>(nodeId.AsPointer<ZigZag::BaseOperator>());
            }
        }
    }
    ImNode::EndDelete();


    //ImNode::PopStyleColor(1);
    //ImNode::PopStyleVar(5);
    ImNode::End();

    ImGui::End();
    //ImGui::PopStyleVar(1);
    ImGui::PopID();
}
