#pragma once

#include <memory>

#include "app/WindowActions.hpp"
#include "gui/MainMenu.hpp"
#include "gui/ObjectInspector.hpp"
#include "gui/Viewport.hpp"

struct GLFWwindow;


class Application
{
public:

    explicit Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    void draw();

private:

    std::shared_ptr<WindowActions> m_windowActions;

    MainMenu m_mainMenu;

    Viewport m_viewport;
    ObjectInspector m_objectInspector{ "Inspector" };

};
