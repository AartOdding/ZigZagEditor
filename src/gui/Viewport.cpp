#include "Viewport.hpp"


#include <iostream>

Viewport::Viewport()
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin("Graph Editor", open);

    ImNode::SetCurrentEditor(m_editorContext);
    ImNode::Begin("Graph Editor");

    ImNode::PushStyleVar(ImNode::StyleVar_NodePadding, {0, 0, 0, 0});
    ImNode::PushStyleVar(ImNode::StyleVar_NodeRounding, 3);
    ImNode::PushStyleVar(ImNode::StyleVar_NodeBorderWidth, 0);
    ImNode::PushStyleVar(ImNode::StyleVar_HoveredNodeBorderWidth, 0);
    ImNode::PushStyleVar(ImNode::StyleVar_SelectedNodeBorderWidth, 0);

    //ImNode::PushStyleColor(ImNode::StyleColor_NodeBg, { 1, 1, 1, 0 });

    if (m_scope)
    {
        for (auto op : m_scope->getChildOperators())
        {
            ImNode::BeginNode((uint64_t)op);
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
                ImNode::BeginPin((uint64_t)inputs[i], ImNode::PinKind::Input);
                ImGui::Dummy({ 20, 20 });
                //ImGui::Text(inputs[i]->getName().c_str());
                ImNode::EndPin();
            }
            for (int i = 0; i < outputs.size(); ++i)
            {
                int y = 30 + i * 30;
                ImGui::SetCursorPos({ localPos.x + 80, localPos.y + y });
                ImNode::BeginPin((uint64_t)outputs[i], ImNode::PinKind::Output);
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
                    ImNode::Link((uint64_t)input, (uint64_t)output, (uint64_t)input);
                }
            }
        }
    }

    if (ImNode::BeginCreate())
    {
        ImNode::PinId inputId, outputId;

        if (ImNode::QueryNewLink(&inputId, &outputId))
        {
            static ZigZag::BaseDataInput* prevIn = nullptr;
            static ZigZag::BaseDataSource* prevOut = nullptr;

            auto input = inputId.AsPointer<ZigZag::BaseDataInput>();
            auto output = outputId.AsPointer<ZigZag::BaseDataSource>();

            if (input != prevIn || output != prevOut)
            {
                prevIn = input;
                prevOut = output;
                std::cout << input << "\t" << output << std::endl;
            }

            // AcceptNewItem returns true if the mouse is released
            if (ImNode::AcceptNewItem())
            {
                std::cout << "connected " << input << "\t" << output << std::endl;
                connect(output, input);
            }
        }
    }
    ImNode::EndCreate();

    //ImNode::PopStyleColor(1);
    ImNode::PopStyleVar(5);
    ImNode::End();

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopID();
}
