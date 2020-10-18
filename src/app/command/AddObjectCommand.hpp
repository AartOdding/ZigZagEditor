#pragma once

#include "Command.hpp"

#include <string>
#include <string_view>

#include <object/ObjectType.hpp>
#include <object/ZObject.hpp>



class AddObjectCommand : public Command
{

public:

	AddObjectCommand(Identifier<ObjectType> objectType, Identifier<ZObject> parentObject);
	~AddObjectCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:
	
	Identifier<ZObject> m_object;
	Identifier<ZObject> m_parentObject;

	bool m_ownsObject{ false };

	std::string m_description;

};
