#pragma once

#include <string>

#include <app/command/Command.hpp>
#include <object/ZObject.hpp>



class RemoveObjectCommand : public Command
{
public:

	RemoveObjectCommand(Identifier<ZObject> objectID);
	~RemoveObjectCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:

	//void recursivelyDisconnect(ZigZag::Object* object);
	//void restoreConnections();

	//std::vector<std::pair<ZigZag::BaseDataSource*, ZigZag::BaseDataInput*>> m_dataDisconnections;
	//std::vector<std::pair<ZigZag::BaseParameter*, ZigZag::BaseParameter*>> m_parameterDisconnections;
	
	Identifier<ZObject> m_objectID;
	Identifier<ZObject> m_parentID;

	std::string m_description;

	bool m_ownsObject;

};
