#pragma once

#include <functional>
#include <vector>


class Action
{
public:

    Action();
    Action(bool state);

    void toggleState();

    bool getState() const;
    void setState(bool);

private:

    std::vector<std::function<void()>> m_callbacks;

    bool m_state;

};
