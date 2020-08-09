#pragma once

#include "Command.hpp"

#include <string>
#include <string_view>

#include <ZigZag/Object.hpp>



class RenameObjectCommand : public Command
{

public:

	RenameObjectCommand(ZigZag::Object* object, std::string_view desiredName);

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& description() final;

private:

	ZigZag::Object* m_object{ nullptr };
	std::string m_initialName;
	std::string m_newName;

	std::string m_description;

};
