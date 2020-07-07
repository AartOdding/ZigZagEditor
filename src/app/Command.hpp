#pragma once

#include <string>


class Command
{
public:

	virtual ~Command() = default;

	virtual void redo() = 0;
	virtual void undo() = 0;

	virtual const std::string& typeName() = 0;
	virtual const std::string& getDescription() = 0;

	virtual bool absorb(const Command& other) { return false; }

};
