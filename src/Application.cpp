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

    m_viewport.setScope(&m_appState.rootOperator);
    m_objectInspector.setScope(&m_appState.rootOperator);
}

void Application::draw()
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

    m_appState.style.push("global");

    m_mainMenu.draw();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    
    if (m_appState.windowActions.viewportWindowOpen.getState())
    {
        bool open = true;
        m_viewport.draw(&open);
        m_appState.windowActions.viewportWindowOpen.setState(open);
    }
    if (m_appState.windowActions.objectInspectorWindowOpen.getState())
    {
        bool open = true;
        m_objectInspector.draw(&open);
        m_appState.windowActions.objectInspectorWindowOpen.setState(open);
    }
    if (m_appState.windowActions.historyWindowOpen.getState())
    {
        bool open = true;
        m_historyView.draw(&open);
        m_appState.windowActions.historyWindowOpen.setState(open);
    }
    if (m_appState.windowActions.renderOrderWindowOpen.getState())
    {
        bool open = true;
        m_renderOrderWindow.draw(&open);
        m_appState.windowActions.renderOrderWindowOpen.setState(open);
    }
    if (m_appState.windowActions.styleEditorWindowOpen.getState())
    {
        bool open = true;
        m_styleEditorWindow.draw(&open);
        m_appState.windowActions.styleEditorWindowOpen.setState(open);
    }
    if (m_appState.windowActions.imguiDemoWindowOpen.getState())
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        m_appState.windowActions.imguiDemoWindowOpen.setState(open);
    }
    if (m_appState.windowActions.imguiStyleWindowOpen.getState())
    {
        bool open = true;
        ImGui::Begin("Imgui Style Editor", &open); // Style editor is not inside of a window by defualt.
        ImGui::ShowStyleEditor();
        ImGui::End();
        m_appState.windowActions.imguiStyleWindowOpen.setState(open);
    }

    m_appState.style.pop("global");

    //ImGui::PopStyleColor(8);
}
