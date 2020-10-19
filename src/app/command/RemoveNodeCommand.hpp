#pragma once

#include <string>

#include <app/command/Command.hpp>
#include <object/Node.hpp>



class RemoveNodeCommand : public Command
{
public:

	RemoveNodeCommand(Identifier<Node> nodeID);
	~RemoveNodeCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:

	//void recursivelyDisconnect(ZigZag::Object* object);
	//void restoreConnections();

	//std::vector<std::pair<ZigZag::BaseDataSource*, ZigZag::BaseDataInput*>> m_dataDisconnections;
	//std::vector<std::pair<ZigZag::BaseParameter*, ZigZag::BaseParameter*>> m_parameterDisconnections;
	
	Identifier<Node> m_nodeID;
	Identifier<Node> m_parentID;

	std::string m_description;

	bool m_ownsNode;

};
