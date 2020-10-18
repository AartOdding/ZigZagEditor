#pragma once

#include <string>

#include <app/command/Command.hpp>
#include <object/ObjectType.hpp>
#include <object/ZObject.hpp>



class AddObjectCommand : public Command
{
public:

	AddObjectCommand(Identifier<ObjectType> typeID, Identifier<ZObject> parentID);
	~AddObjectCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:
	
	Identifier<ZObject> m_objectID;
	Identifier<ZObject> m_parentID;

	std::string m_description;

	bool m_ownsObject;

};
