#include "Application.hpp"
#include "types.hpp"
#include "OpenSansRegular.hpp"

#include "library/dataInputs/GenericInput.hpp"
#include "library/dataSources/TextureData.hpp"

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/TParameter.hpp>

#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>

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


void Application::updateBetweenFrames()
{
    if (m_styleOutdated)
    {
        ImGui::GetIO().Fonts->Clear();
        ImGui_ImplOpenGL3_DestroyFontsTexture();
        ImFontConfig fontConfig;
        fontConfig.FontDataOwnedByAtlas = false;
        ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)OpenSans_Regular_ttf, OpenSans_Regular_ttf_len, m_fontSize * m_dpiScaling, &fontConfig);
        ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)OpenSans_Regular_ttf, OpenSans_Regular_ttf_len, m_fontSizeCode * m_dpiScaling, &fontConfig);
        ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);
        ImGui_ImplOpenGL3_CreateFontsTexture();
        ImGui::GetStyle() = ImGuiStyle();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(m_dpiScaling);
        m_styleOutdated = false;
    }
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
            m_luaEditorWindows[luaBehaviour] = std::make_unique<LuaEditorWindow>(*luaBehaviour);
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

int Application::getFontSize() const
{
    return m_fontSize;
}

void Application::setFontSize(int fontSize)
{
    int clampedFontSize = std::clamp<int>(fontSize, 6, 52);
    m_styleOutdated |= m_fontSize != clampedFontSize;
    m_fontSize = clampedFontSize;
}

int Application::getCodeSize() const
{
    return m_fontSize;
}

void Application::setCodeSize(int fontSize)
{
    int clampedCodeSize = std::clamp<int>(fontSize, 6, 52);
    m_styleOutdated |= m_fontSizeCode != clampedCodeSize;
    m_fontSizeCode = clampedCodeSize;
}

void Application::setDpiScaling(float dpiScaling)
{
    float clampedDpiScaling = std::clamp<float>(dpiScaling, 0.25f, 4.0f);
    m_styleOutdated |= m_dpiScaling != clampedDpiScaling;
    m_dpiScaling = clampedDpiScaling;
}

float Application::getDpiScaling() const
{
    return m_dpiScaling;
}
