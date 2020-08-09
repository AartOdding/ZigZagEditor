#pragma once

#include <ZigZag/Object.hpp>
#include <ZigZag/ObjectSelection.hpp>

#include "app/ApplicationState.hpp"
#include "app/command/CommandStack.hpp"



class HistoryView
{
public:

    HistoryView(std::string_view windowName, ApplicationState* appState);

    void draw(bool* p_open);

private:

    std::string m_windowName;

    ApplicationState* m_appState;

};
