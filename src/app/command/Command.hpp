#pragma once

#include <memory>
#include <string>
#include <vector>


class Command
{
public:

	virtual ~Command() = default;

	virtual const std::string& getCommandName() = 0;
	virtual const std::string& description() = 0;

	virtual bool redo() = 0;
	virtual bool undo() = 0;

};
