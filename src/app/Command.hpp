#pragma once

#include <memory>
#include <string>
#include <vector>


class Command
{
	friend class CommandStack;

public:

	virtual ~Command() = default;

	virtual const std::string& typeName() = 0;
	virtual const std::string& description() = 0;

protected:

	virtual void redo() = 0;
	virtual void undo() = 0;

	template <typename T, typename ... Args>
	void addSubCommand(Args&& ... args)
	{
		m_subCommands.push_back(std::make_unique<T>(std::forward<Args>(args) ...));
	}

private:

	std::vector<std::unique_ptr<Command>> m_subCommands;

};
