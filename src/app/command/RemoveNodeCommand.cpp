#include <cassert>
#include <stdexcept>

#include <app/command/RemoveNodeCommand.hpp>
#include <interop/ObjectInterop.hpp>



RemoveNodeCommand::RemoveNodeCommand(Identifier<Node> nodeID)
	: m_nodeID(nodeID)
	, m_ownsNode(false)
{
	assert(IdentityMap<Node>::get(nodeID));
	assert(IdentityMap<Node>::get(nodeID)->getParent());

	// TODO LOGGING: log error if object does not exist, or does not have a parent.

	auto objectPtr = IdentityMap<Node>::get(nodeID);
	if (objectPtr)
	{
		auto parentPtr = objectPtr->getParent();
		if (parentPtr)
		{
			// If the object has a parent we need to store it for proper undoing
			m_parentID = parentPtr->getIdentifier();
		}
		// else log error
	}
	// else log error

	m_description = "Removed node <" + std::to_string(static_cast<std::uint64_t>(m_nodeID)) + ">";
}

RemoveNodeCommand::~RemoveNodeCommand()
{
	if (m_ownsNode)
	{
		Host::destroyNode(m_nodeID);
	}
}

bool RemoveNodeCommand::redo()
{
	if (m_nodeID && m_parentID)
	{
		if (Host::setNodeParent(m_nodeID, Identifier<Node>()))
		{
			m_ownsNode = true;
			return true;
		}
	}
	// TODO LOGGING: log info that a command was unsuccesful
	return false;
}

bool RemoveNodeCommand::undo()
{
	if (Host::setNodeParent(m_nodeID, m_parentID))
	{
		m_ownsNode = false;
		return true;
	}
	return false;
}

const std::string& RemoveNodeCommand::getCommandName()
{
	static const std::string name = "RemoveNodeCommand";
	return name;
}

const std::string& RemoveNodeCommand::description()
{
	return m_description;
}
/*
void RemoveObjectCommand::recursivelyDisconnect(ZigZag::Object* object)
{
	// Important about this function is that when a connection between two
	// objects exists, that are both descendants of the object that is being
	// removed, we do not need to actually disconnect them. This is the
	// reason of the many if isChildOf statements.

	if (object)
	{
		if (auto input = dynamic_cast<ZigZag::BaseDataInput*>(object))
		{
			if (auto source = input->getConnectedOutput())
			{
				// Input is always a child of m_object, no need to check.
				if (!source->isChildOf(m_object, false))
				{
					ZigZag::disconnect(source, input);
					m_dataDisconnections.emplace_back(source, input);
				}
			}
		}
		else if (auto source = dynamic_cast<ZigZag::BaseDataSource*>(object))
		{
			while (source->getConnectedInputs().size() > 0)
			{
				auto input = source->getConnectedInputs().back();

				// Source is always a child of m_object, no need to check.
				if (!input->isChildOf(m_object, false))
				{
					ZigZag::disconnect(source, input);
					m_dataDisconnections.emplace_back(source, input);
				}
			}
		}
		else if (auto parameter = dynamic_cast<ZigZag::BaseParameter*>(object))
		{
			if (auto source = parameter->getConnectedOutput())
			{
				// In this case parameter is the input.
				// It is always a child of m_object, no need to check.
				if (!source->isChildOf(m_object, false))
				{
					disconnect(source, parameter);
					m_parameterDisconnections.emplace_back(source, parameter);
				}
			}
			while (parameter->getConnectedInputs().size() > 0)
			{
				auto input = parameter->getConnectedInputs().back();

				// In this case parameter is the source.
				// It is always a child of m_object, no need to check.
				if (!source->isChildOf(m_object, false))
				{
					disconnect(parameter, input);
					m_parameterDisconnections.emplace_back(parameter, input);
				}
			}
		}

		// Recursively do the same for all children.
		for (auto child : object->getChildObjects())
		{
			recursivelyDisconnect(child);
		}
	}
}

void RemoveObjectCommand::restoreConnections()
{
	for (auto [source, input] : m_dataDisconnections)
	{
		connect(source, input);
	}
	for (auto [source, input] : m_parameterDisconnections)
	{
		connect(source, input);
	}
	m_dataDisconnections.clear();
	m_parameterDisconnections.clear();
}
*/