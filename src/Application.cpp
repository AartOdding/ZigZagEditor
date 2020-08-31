#include "Application.hpp"
#include "types.hpp"

#include "library/dataInputs/GenericInput.hpp"
#include "library/dataSources/TextureData.hpp"

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/TParameter.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>


namespace zz = ZigZag;



Application::Application()
{
    registerTypes();

    m_nodeEditorWindow.setScope(&m_appState.rootOperator);
    m_hierarchyWindow.setScope(&m_appState.rootOperator);
}

void Application::update()
{
    //auto tabColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    //auto tabColorActiveNoFocus = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
    //auto backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
    //ImGui::PushStyleColor(ImGuiCol_Tab, tabColor);
    //ImGui::PushStyleColor(ImGuiCol_TabUnfocused, tabColor);
    //ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, tabColorActiveNoFocus);
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
    //ImGui::PushStyleColor(ImGuiCol_TitleBg, backgroundColor);
    //ImGui::PushStyleColor(ImGuiCol_TitleBgActive, backgroundColor);
    //ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, backgroundColor);
    //ImGui::PushStyleColor(ImGuiCol_Separator, backgroundColor);

    m_appState.style.push("Application");

    m_mainMenu.draw();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    
    m_nodeEditorWindow.update();
    m_hierarchyWindow.update();
    m_historyWindow.update();
    m_renderOrderWindow.update();
    m_styleEditorWindow.update();
    m_luaEditorWindow.update();

    if (m_ImGuiDemoWindowOpen)
    {
        ImGui::ShowDemoWindow(&m_ImGuiDemoWindowOpen);
    }
    if (m_ImGuiStyleEditorWindowOpen)
    {
        ImGui::Begin("Imgui Style Editor", &m_ImGuiStyleEditorWindowOpen);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    //if (m_appState.windowActions.imguiDemoWindowOpen.getState())
    //{
    //    bool open = true;
    //    ImGui::ShowDemoWindow(&open);
    //    m_appState.windowActions.imguiDemoWindowOpen.setState(open);
    //}
    //if (m_appState.windowActions.imguiStyleWindowOpen.getState())
    //{
    //    bool open = true;
    //    ImGui::Begin("Imgui Style Editor", &open); // Style editor is not inside of a window by defualt.
    //    ImGui::ShowStyleEditor();
    //    ImGui::End();
    //    m_appState.windowActions.imguiStyleWindowOpen.setState(open);
    //}

    m_appState.style.pop("Application");

    //ImGui::PopStyleColor(8);
}


void Application::openWindow(WindowType type)
{
    switch (type)
    {
        case WindowType::NodeEditor: 
            m_nodeEditorWindow.open();
            break;
        case WindowType::Hierarchy: 
            m_hierarchyWindow.open();
            break;
        case WindowType::History: 
            m_historyWindow.open();
            break;
        case WindowType::RenderOrder: 
            m_renderOrderWindow.open();
            break;
        case WindowType::StyleEditor: 
            m_styleEditorWindow.open();
            break;
        case WindowType::ImGuiDemo: 
            m_ImGuiDemoWindowOpen = true;
            break;
        case WindowType::ImGuiStyleEditor: 
            m_ImGuiStyleEditorWindowOpen = true;
            break;
    }
}

int Application::windowOpenCount(WindowType type)
{
    switch (type)
    {
    case WindowType::NodeEditor: return m_nodeEditorWindow.isOpen();
    case WindowType::Hierarchy: return m_hierarchyWindow.isOpen();
    case WindowType::History: return m_historyWindow.isOpen();
    case WindowType::RenderOrder: return m_renderOrderWindow.isOpen();
    case WindowType::StyleEditor: return m_styleEditorWindow.isOpen();
    case WindowType::ImGuiDemo: return m_ImGuiDemoWindowOpen;
    case WindowType::ImGuiStyleEditor: return m_ImGuiStyleEditorWindowOpen;
    }
    return 0;
}
