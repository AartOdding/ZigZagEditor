#include <stdexcept>

#include <app/command/AddNodeCommand.hpp>
#include <interop/ObjectInterop.hpp>



AddNodeCommand::AddNodeCommand(Identifier<Template> templateID, Identifier<Node> parentID)
	: m_parentID(parentID)
	, m_ownsNode(false)
{
	assert(IdentityMap<Template>::get(templateID));
	assert(IdentityMap<Node>::get(parentID));

	// TODO LOGGING: log error if object type does not exist, or parent is null.

	if (IdentityMap<Template>::get(templateID))
	{
		// Generate the object without a parent first, set parent in redo
		m_nodeID = Host::createNode(templateID, Identifier<Node>());
		m_ownsNode = static_cast<bool>(m_nodeID);
	}
	// else log error

	m_description = "Added node <" + std::to_string(static_cast<std::uint64_t>(m_nodeID)) + ">";
}

AddNodeCommand::~AddNodeCommand()
{
	if (m_ownsNode)
	{
		Host::destroyNode(m_nodeID);
	}
}

bool AddNodeCommand::redo()
{
	if (m_nodeID && m_parentID)
	{
		if (Host::setNodeParent(m_nodeID, m_parentID))
		{
			m_ownsNode = false;
			return true;
		}
	}
	// TODO LOGGING: log info that a command was unsuccesful
	return false;
}

bool AddNodeCommand::undo()
{
	if (Host::setNodeParent(m_nodeID, Identifier<Node>()))
	{
		m_ownsNode = true;
		return true;
	}
	return false;
}

const std::string& AddNodeCommand::getCommandName()
{
	static const std::string name = "AddNodeCommand";
	return name;
}

const std::string& AddNodeCommand::description()
{
	return m_description;
}
