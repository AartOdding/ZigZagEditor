#pragma once

#include <string>

#include <app/command/Command.hpp>
#include <object/Node.hpp>
#include <object/NodeTemplate.hpp>



class AddNodeCommand : public Command
{
public:

	AddNodeCommand(Identifier<NodeTemplate> templateID, Identifier<Node> parentID);
	~AddNodeCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:
	
	Identifier<Node> m_nodeID;
	Identifier<Node> m_parentID;

	std::string m_description;

	bool m_ownsNode;

};
