#pragma once

#include <memory>

#include "app/ApplicationState.hpp"

#include "gui/HistoryView.hpp"
#include "gui/MainMenu.hpp"
#include "gui/ObjectInspector.hpp"
#include "gui/RenderOrderWindow.hpp"
#include "gui/Viewport.hpp"



class Application
{
public:

    explicit Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    void draw();

private:

    ApplicationState m_appState;

    MainMenu m_mainMenu{ &m_appState };

    Viewport m_viewport{ "Editor", &m_appState };
    ObjectInspector m_objectInspector{ "Inspector", &m_appState };
    HistoryView m_historyView{ "History", &m_appState };
    RenderOrderWindow m_renderOrderWindow{ "Render Order", &m_appState };

};
