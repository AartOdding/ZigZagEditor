#include "Action.hpp"




Action::Action()
    : m_state(false)
{

}


Action::Action(bool state)
    : m_state(state)
{
    
}


void Action::toggleState()
{
    m_state = !m_state;
}


bool Action::getState() const
{
    return m_state;
}


void Action::setState(bool state)
{
    m_state = state;
}
