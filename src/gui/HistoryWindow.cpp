#include "HistoryWindow.hpp"

#include <imgui.h>


using namespace ImGui;
using namespace ZigZag;

HistoryWindow::HistoryWindow(std::string_view windowName, ApplicationState* appState)
	: Window(windowName),
	  m_appState(appState)
{

}


void HistoryWindow::draw()
{
    if (Button("Undo"))
    {
        m_appState->commandStack.undo();
    }
    SameLine();
    if (Button("Redo"))
    {
        m_appState->commandStack.redo();
    }
    SameLine();
    if (Button("Clear"))
    {
        m_appState->commandStack.clear();
    }

    //ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    //ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
    //ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_Tab));
    //ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));
    //ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));

    if (ListBoxHeader("##history", ImGui::GetContentRegionAvail()))
    {
        const auto& commands = m_appState->commandStack.getStack();
        auto current = m_appState->commandStack.getIndex();

        for (int i = commands.size()-1; i >= 0; --i)
        {
            if (i > current)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            }
            Selectable(commands[i]->description().c_str());

            if (i > current)
            {
                ImGui::PopStyleColor();
            }
        }
        //Selectable("Initial State", m_appState->commandStack.getIndex() == -1);
        ListBoxFooter();
    }
}
