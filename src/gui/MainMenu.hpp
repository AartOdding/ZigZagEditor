#pragma once

#include <memory>

#include "../app/WindowActions.hpp"


class MainMenu
{
public:

    MainMenu(const std::shared_ptr<WindowActions>& windowActions);

    MainMenu(const MainMenu&) = delete;
    MainMenu(MainMenu&&) = default;

    void draw();

private:

    std::shared_ptr<WindowActions> m_windowActions;

};
