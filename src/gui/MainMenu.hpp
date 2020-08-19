#pragma once

#include <memory>

#include "app/ApplicationState.hpp"

class Application;


class MainMenu
{
public:

    MainMenu(Application* application, ApplicationState* appState);

    MainMenu(const MainMenu&) = delete;
    MainMenu(MainMenu&&) = default;

    void draw();

private:

    Application* m_application;
    ApplicationState* m_appState;


};
