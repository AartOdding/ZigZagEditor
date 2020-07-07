#pragma once

#include <memory>
#include <vector>

#include "Command.hpp"



class CommandStack
{
public:

	CommandStack() = default;
	CommandStack(const CommandStack&) = delete;

	void pushCommand(std::unique_ptr<Command>&& command);

	bool undo();
	bool redo();

	bool canRedo() const;
	bool canUndo() const;

	void clear();

	int getIndex() const;
	const std::vector<std::unique_ptr<Command>>& getStack() const;

	bool isAtTop() const;


private:

	void resetTopToCurrent();

	// Index to one above the last done command.
	int m_index = 0;

	std::vector<std::unique_ptr<Command>> m_commandStack;

};
