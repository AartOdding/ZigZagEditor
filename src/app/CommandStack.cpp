#include "CommandStack.hpp"



void CommandStack::pushCommand(std::unique_ptr<Command>&& command)
{
	if (command)
	{
		resetTopToCurrent();

		if (!m_commandStack.empty() && m_commandStack.back()->absorb(*command))
		{
			command.reset(nullptr);
		}
		else
		{
			m_commandStack.push_back(std::move(command));
			m_commandStack.back()->redo();
			m_index++;
		}
	}
}

bool CommandStack::undo()
{
	if (canUndo())
	{
		m_commandStack[--m_index]->undo();
		return true;
	}
	return false;
}

bool CommandStack::redo()
{
	if (canRedo())
	{
		m_commandStack[m_index++]->redo();
		return true;
	}
	return false;
}

bool CommandStack::canRedo() const
{
	return m_index < m_commandStack.size();
}

bool CommandStack::canUndo() const
{
	return m_index > 0;
}

void CommandStack::clear()
{
	m_commandStack.clear();
	m_index = 0;
}

int CommandStack::getIndex() const
{
	// -1 because index points one ahead of the current active command.
	return m_index - 1;
}

const std::vector<std::unique_ptr<Command>>& CommandStack::getStack() const
{
	return m_commandStack;
}

bool CommandStack::isAtTop() const
{
	return m_index == m_commandStack.size();
}

void CommandStack::resetTopToCurrent()
{
	if (m_index < m_commandStack.size())
	{
		m_commandStack.resize(m_index);
	}
}
