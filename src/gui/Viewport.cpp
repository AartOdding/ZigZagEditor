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
            ImNode::BeginPin(1, ImNode::PinKind::Input);
            ImGui::Text(" ");
            ImNode::EndPin();
            ImGui::SameLine();
            ImNode::BeginPin(2, ImNode::PinKind::Output);
            ImGui::Text(" ");
            ImNode::EndPin();
            ImNode::EndNode();
        }
    }

    ImNode::End();

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopID();
}
