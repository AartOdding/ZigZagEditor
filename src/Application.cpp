#include "Application.hpp"
#include "types.hpp"

#include "app/Directories.hpp"
#include "library/dataInputs/GenericInput.hpp"
#include "library/dataSources/TextureData.hpp"

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/LuaBehaviour.hpp>
#include <ZigZag/TParameter.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>


namespace zz = ZigZag;



Application::Application()
{
    registerTypes();
    ZigZag::LuaBehaviour::loadDefaultScript((Directories::resourcesDir() / "LuaBehaviourDefaultScript.lua").string());

    m_nodeEditorWindow.setScope(&m_appState.rootOperator);
    m_hierarchyWindow.setScope(&m_appState.rootOperator);
}

void Application::update()
{
    m_appState.style.push("Application");

    m_mainMenu.draw();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    
    m_nodeEditorWindow.update();
    m_hierarchyWindow.update();
    m_historyWindow.update();
    m_renderOrderWindow.update();
    m_styleEditorWindow.update();

    for (auto it = m_luaEditorWindows.begin(); it != m_luaEditorWindows.end();) // Important: no increment
    {
        it->second->update();
        it->second->isOpen() ? ++it : it = m_luaEditorWindows.erase(it);
    }

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

    m_appState.style.pop("Application");
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

void Application::openLuaEditorWindow(ZigZag::LuaBehaviour* luaBehaviour)
{
    if (luaBehaviour)
    {
        auto it = m_luaEditorWindows.find(luaBehaviour);

        if (it != m_luaEditorWindows.end())
        {
            it->second->open();
            // also set focus
        }
        else
        {
            m_luaEditorWindows[luaBehaviour] = std::make_unique<LuaEditorWindow>(*luaBehaviour, &m_appState);
        }
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
        case WindowType::LuaEditor: return m_luaEditorWindows.size();
        case WindowType::ImGuiDemo: return m_ImGuiDemoWindowOpen;
        case WindowType::ImGuiStyleEditor: return m_ImGuiStyleEditorWindowOpen;
    }
    return 0;
}

ApplicationState* Application::getAppState()
{
    return &m_appState;
}
