#pragma once

#include "Action.hpp"


struct WindowActions
{
    Action viewportWindowOpen{ true };
    Action objectInspectorWindowOpen{ true };
    Action historyWindowOpen{ true };
    Action renderOrderWindowOpen{ true };
    Action imguiDemoWindowOpen;
    Action imguiStyleWindowOpen;
};
