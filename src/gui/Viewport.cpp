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

    if (m_scope)
    {
        for (auto op : m_scope->getChildOperators())
        {
            ImNode::BeginNode((uint64_t)op);
            ImGui::Text(op->getName().c_str());

            auto& inputs = op->getDataInputs();
            auto& outputs = op->getDataSources();

            auto max = std::max(inputs.size(), outputs.size());

            for (int i = 0; i < max; ++i)
            {
                if (i < inputs.size())
                {
                    ImNode::BeginPin((uint64_t)inputs[i], ImNode::PinKind::Input);
                    ImGui::Text(inputs[i]->getName().c_str());
                    ImNode::EndPin();
                }
                else
                {
                    ImGui::Text(" ");
                }
                ImGui::SameLine();

                if (i < outputs.size())
                {
                    ImNode::BeginPin((uint64_t)outputs[i], ImNode::PinKind::Output);
                    ImGui::Text(outputs[i]->getName().c_str());
                    ImNode::EndPin();
                }
            }
            ImNode::EndNode();
        }
    }

    ImNode::End();

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopID();
}
