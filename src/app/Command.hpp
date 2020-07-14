#pragma once

#include <memory>
#include <string>
#include <vector>


class Command
{
public:

	virtual ~Command() = default;

	virtual const std::string& typeName() = 0;
	virtual const std::string& description() = 0;

	virtual void redo() = 0;
	virtual void undo() = 0;

};
