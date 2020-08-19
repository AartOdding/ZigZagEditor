#pragma once

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectSelection.hpp>

#include "app/ApplicationState.hpp"
#include "app/command/CommandStack.hpp"
#include "gui/Window.hpp"



class HistoryWindow : public Window
{
public:

    HistoryWindow(std::string_view windowName, ApplicationState* appState);

protected:

    void draw() override;

private:

    ApplicationState* m_appState;

};
