#pragma once

#include <memory>
#include <unordered_map>

#include "app/ApplicationState.hpp"

#include "gui/MainMenu.hpp"

#include "gui/HierarchyWindow.hpp"
#include "gui/HistoryWindow.hpp"
#include "gui/LuaEditorWindow.hpp"
#include "gui/NodeEditorWindow.hpp"
#include "gui/RenderOrderWindow.hpp"
#include "gui/StyleEditorWindow.hpp"

namespace ZigZag
{
    class LuaBehaviour;
}

class Application
{
public:

    enum class WindowType
    {
        NodeEditor,
        Hierarchy,
        History,
        LuaEditor,
        RenderOrder,
        StyleEditor,
        ImGuiDemo,
        ImGuiStyleEditor
    };


    explicit Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    void update();
    void updateBetweenFrames();

    void openWindow(WindowType type);
    void openLuaEditorWindow(ZigZag::LuaBehaviour* luaBehaviour);

    int windowOpenCount(WindowType type);

    int getFontSize() const;
    void setFontSize(int fontSize);

    int getCodeSize() const;
    void setCodeSize(int codeSize);

    // The framebuffer scale argument should be used when the scale between the window size and
    // the framebuffer size is not 1. This is for instance the case on mac retina displays.
    void setDpiScaling(float desiredScale, float frameBufferScale = 1.0f);
    float getDpiScaling() const;


private:

    ApplicationState m_appState;

    MainMenu m_mainMenu{ this, &m_appState };

    HierarchyWindow m_hierarchyWindow{ "Project Hierarchy", this, &m_appState };
    HistoryWindow m_historyWindow{ "History", &m_appState };
    NodeEditorWindow m_nodeEditorWindow{ "Node Editor", &m_appState };
    RenderOrderWindow m_renderOrderWindow{ "Render Order", &m_appState };
    StyleEditorWindow m_styleEditorWindow{ "Style Editor", &m_appState };

    std::unordered_map<ZigZag::LuaBehaviour*, std::unique_ptr<LuaEditorWindow>> m_luaEditorWindows;

    bool m_ImGuiDemoWindowOpen = false;
    bool m_ImGuiStyleEditorWindowOpen = false;
    bool m_styleOutdated = false;

    int m_fontSize = 18;
    int m_codeSize = 18;
    float m_desiredScaling = 1.0f;
    float m_frameBufferScaling = 1.0f;
    float m_dpiScaling = 1.0f;

};
