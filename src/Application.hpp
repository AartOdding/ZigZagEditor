#pragma once

#include <memory>

#include "app/ApplicationState.hpp"

#include "gui/HistoryWindow.hpp"
#include "gui/MainMenu.hpp"
#include "gui/HierarchyWindow.hpp"
#include "gui/RenderOrderWindow.hpp"
#include "gui/StyleEditorWindow.hpp"
#include "gui/NodeEditorWindow.hpp"



class Application
{
public:

    enum class WindowType
    {
        NodeEditor,
        Hierarchy,
        History,
        RenderOrder,
        StyleEditor,
        ImGuiDemo,
        ImGuiStyleEditor
    };


    explicit Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    void update();

    void openWindow(WindowType type);
    int windowOpenCount(WindowType type);

private:

    ApplicationState m_appState;

    MainMenu m_mainMenu{ this, &m_appState };

    NodeEditorWindow m_nodeEditorWindow{ "Node Editor", &m_appState };
    HierarchyWindow m_hierarchyWindow{ "Project Hierarchy", &m_appState };
    HistoryWindow m_historyWindow{ "History", &m_appState };
    RenderOrderWindow m_renderOrderWindow{ "Render Order", &m_appState };
    StyleEditorWindow m_styleEditorWindow{ "Style Editor", &m_appState };

    bool m_ImGuiDemoWindowOpen = false;
    bool m_ImGuiStyleEditorWindowOpen = false;
};
