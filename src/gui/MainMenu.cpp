#include "MainMenu.hpp"
#include "Application.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>



MainMenu::MainMenu(Application* application, ApplicationState* appState)
    : m_application(application)
    , m_appState(appState)
{

}


void MainMenu::draw()
{
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
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
        ImGui::MenuItem("Recentre", nullptr, false, false);

        ImGui::Separator();

        if (ImGui::MenuItem("Node Editor", nullptr, m_application->windowOpenCount(Application::WindowType::NodeEditor)))
        {
            m_application->openWindow(Application::WindowType::NodeEditor);
        }
        if (ImGui::MenuItem("Project Hierarchy", nullptr, m_application->windowOpenCount(Application::WindowType::Hierarchy)))
        {
            m_application->openWindow(Application::WindowType::Hierarchy);
        }
        if (ImGui::MenuItem("History", nullptr, m_application->windowOpenCount(Application::WindowType::History)))
        {
            m_application->openWindow(Application::WindowType::History);
        }
        if (ImGui::MenuItem("Render Order", nullptr, m_application->windowOpenCount(Application::WindowType::RenderOrder)))
        {
            m_application->openWindow(Application::WindowType::RenderOrder);
        }
        if (ImGui::MenuItem("Style Editor", nullptr, m_application->windowOpenCount(Application::WindowType::StyleEditor)))
        {
            m_application->openWindow(Application::WindowType::StyleEditor);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("ImGui Demo Window", nullptr, m_application->windowOpenCount(Application::WindowType::ImGuiDemo)))
        {
            m_application->openWindow(Application::WindowType::ImGuiDemo);
        }
        if (ImGui::MenuItem("ImGui Style Editor", nullptr, m_application->windowOpenCount(Application::WindowType::ImGuiStyleEditor)))
        {
            m_application->openWindow(Application::WindowType::ImGuiStyleEditor);
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings"))
    {
        if (ImGui::BeginMenu("Monitors"))
        {
            int monitorCount;
            GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

            for (int i = 0; i < monitorCount; ++i)
            {
                auto name = glfwGetMonitorName(monitors[i]);
                int w, h;
                float sx, sy;
                glfwGetMonitorPhysicalSize(monitors[i], &w, &h);
                glfwGetMonitorContentScale(monitors[i], &sx, &sy);
                if (name)
                {
                    ImGui::PushID(i);
                    ImGui::Text(name);
                    ImGui::Text(std::to_string(w).c_str());
                    ImGui::Text(std::to_string(h).c_str());
                    ImGui::Text(std::to_string(sx).c_str());
                    ImGui::Text(std::to_string(sy).c_str());

                    float sliderVal = 1.0f;
                    ImGui::SliderFloat("##slider", &sliderVal, 0.25, 3, "%.2f");
                    ImGui::PopID();
                }
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
    //ImGui::PopStyleVar();
}
