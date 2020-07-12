#pragma once

#include <memory>
#include <vector>

#include "Command.hpp"



class CommandStack
{
public:

	CommandStack() = default;
	CommandStack(const CommandStack&) = delete;

	template <typename T, typename ... Args>
	void push(Args&& ... args)
	{
		pushCommand(std::make_unique<T>(std::forward<Args>(args) ...));
	}

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
	void pushCommand(std::unique_ptr<Command>&& command);

	// Index to one above the last done command.
	int m_index = 0;

	std::vector<std::unique_ptr<Command>> m_commandStack;

};
