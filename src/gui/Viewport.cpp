#include "Viewport.hpp"




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
    ImNode::PushStyleVar(ImNode::StyleVar_NodePadding, 0);

    if (m_scope)
    {
        for (auto op : m_scope->getChildOperators())
        {
            ImNode::BeginNode((uint64_t)op);
            auto localPos = ImGui::GetCursorPos();
            ImGui::Dummy({ 100, 100 });
            ImGui::SetCursorPos(localPos);
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

            //for (int i = 0; i < max; ++i)
            //{
            //    if (i < inputs.size())
            //    {
            //        ImNode::BeginPin((uint64_t)inputs[i], ImNode::PinKind::Input);
            //        ImGui::Text(inputs[i]->getName().c_str());
            //        ImNode::EndPin();
            //    }
            //    else
            //    {
            //        ImGui::Text(" ");
            //    }
            //    ImGui::SameLine();

            //    if (i < outputs.size())
            //    {
            //        ImNode::BeginPin((uint64_t)outputs[i], ImNode::PinKind::Output);
            //        ImGui::Text(outputs[i]->getName().c_str());
            //        ImNode::EndPin();
            //    }
            //}
            ImNode::EndNode();
        }
    }

    ImNode::PopStyleVar(1);
    ImNode::End();

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopID();
}
