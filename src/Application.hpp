#pragma once

#include <memory>

#include "app/ApplicationState.hpp"

#include "gui/HistoryView.hpp"
#include "gui/MainMenu.hpp"
#include "gui/ObjectInspector.hpp"
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

    MainMenu m_mainMenu;

    Viewport m_viewport;
    ObjectInspector m_objectInspector{ "Inspector", &m_appState };
    HistoryView m_historyView{ "History", &m_appState };

};
