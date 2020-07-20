#pragma once

#include <memory>

#include "app/ApplicationState.hpp"


class MainMenu
{
public:

    MainMenu(ApplicationState* appState);

    MainMenu(const MainMenu&) = delete;
    MainMenu(MainMenu&&) = default;

    void draw();

private:

    ApplicationState* m_appState;

};
