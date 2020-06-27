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


void Viewport::draw(bool* p_open)
{
    ImGui::PushID(this);
    ImGui::Begin("Graph Editor", p_open);

    ImNode::SetCurrentEditor(m_editorContext);
    ImNode::Begin("Graph Editor");

        ImNode::BeginNode(0);
        ImGui::Text("Node A");
        ImNode::BeginPin(1, ImNode::PinKind::Input);
        ImGui::Text("-> In");
        ImNode::EndPin();
        ImGui::SameLine();
        ImNode::BeginPin(2, ImNode::PinKind::Output);
        ImGui::Text("Out ->");
        ImNode::EndPin();
        ImNode::EndNode();

    ImNode::End();

    ImGui::End();
    ImGui::PopID();
}
