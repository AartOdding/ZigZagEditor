#include "MainMenu.hpp"

#include <imgui.h>



MainMenu::MainMenu(ApplicationState* appState)
    : m_appState(appState)
{

}


void MainMenu::draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save", "CTRL+S")) {}
        if (ImGui::MenuItem("Open", "CTRL+O")) {}
        if (ImGui::MenuItem("New", "CTRL+N")) {}
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Recentre", nullptr, false, m_appState->windowActions.viewportWindowOpen.getState());
        ImGui::Separator();
        if (ImGui::MenuItem("Editor", nullptr, m_appState->windowActions.viewportWindowOpen.getState()))
        {
            m_appState->windowActions.viewportWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("Project Hierarchy", nullptr, m_appState->windowActions.objectInspectorWindowOpen.getState()))
        {
            m_appState->windowActions.objectInspectorWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("History", nullptr, m_appState->windowActions.historyWindowOpen.getState()))
        {
            m_appState->windowActions.historyWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("Render Order", nullptr, m_appState->windowActions.renderOrderWindowOpen.getState()))
        {
            m_appState->windowActions.renderOrderWindowOpen.setState(true);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("ImGui Demo", nullptr, m_appState->windowActions.imguiDemoWindowOpen.getState()))
        {
            m_appState->windowActions.imguiDemoWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("ImGui Style", nullptr, m_appState->windowActions.imguiStyleWindowOpen.getState()))
        {
            m_appState->windowActions.imguiStyleWindowOpen.setState(true);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();
}
