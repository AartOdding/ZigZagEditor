#include "RemoveObjectCommand.hpp"

#include <stdexcept>


using namespace ZigZag;


RemoveObjectCommand::RemoveObjectCommand(ZigZag::Object* object)
	: m_object(object)
{
	if (!m_object || !m_object->getParent() || !m_object->getDeleteByParent())
	{
		throw std::runtime_error("Cannot remove object.");
	}
	m_parentObject = m_object->getParent();
	m_description = "Remove " + m_object->getName() + " from " + m_object->getParent()->getName();
}

RemoveObjectCommand::~RemoveObjectCommand()
{
	if (m_object && m_ownsObject)
	{
		delete m_object;
	}
}

void RemoveObjectCommand::redo()
{
	// Disconnects everything and keeps track of all the things disconnected.
	recursivelyDisconnect(m_object);

	// The actual removal of the object from the parent:
	m_object->setDeleteByParent(false);
	m_ownsObject = true;
	m_object->setParent(nullptr);
}

void RemoveObjectCommand::undo()
{
	// First restore all connections.
	restoreConnections();

	// Actually set the parent back.
	m_object->setParent(m_parentObject);
	m_object->setDeleteByParent(true);
	m_ownsObject = false;
}

const std::string& RemoveObjectCommand::typeName()
{
	static const std::string name = "RemoveObjectCommand";
	return name;
}

const std::string& RemoveObjectCommand::description()
{
	return m_description;
}

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
					disconnect(source, input);
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
					disconnect(source, input);
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
