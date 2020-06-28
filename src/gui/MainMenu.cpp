#include "MainMenu.hpp"

#include <imgui.h>



MainMenu::MainMenu(const std::shared_ptr<WindowActions>& windowActions)
    : m_windowActions(windowActions)
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
        ImGui::MenuItem("Recentre", nullptr, false, m_windowActions->viewportWindowOpen.getState());
        ImGui::Separator();
        if (ImGui::MenuItem("Viewport", nullptr, m_windowActions->viewportWindowOpen.getState()))
        {
            m_windowActions->viewportWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("Object Inspector", nullptr, m_windowActions->objectInspectorWindowOpen.getState()))
        {
            m_windowActions->objectInspectorWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("ImGui Demo", nullptr, m_windowActions->imguiDemoWindowOpen.getState()))
        {
            m_windowActions->imguiDemoWindowOpen.setState(true);
        }
        if (ImGui::MenuItem("ImGui Style", nullptr, m_windowActions->imguiStyleWindowOpen.getState()))
        {
            m_windowActions->imguiStyleWindowOpen.setState(true);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();
}
