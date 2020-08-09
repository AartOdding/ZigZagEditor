#include "RenameObjectCommand.hpp"

#include <stdexcept>


using namespace ZigZag;


RenameObjectCommand::RenameObjectCommand(ZigZag::Object* object, std::string_view desiredName)
	: m_object(object)
{
	if (!m_object)
	{
		throw std::runtime_error("Cannot rename null object.");
	}
	m_initialName = m_object->getName();
	m_newName = m_object->getClosestPotentialName(desiredName);

	m_description = "Rename " + m_initialName + " to " + m_newName;
}

void RenameObjectCommand::redo()
{
	m_object->setName(m_newName);
}

void RenameObjectCommand::undo()
{
	m_object->setName(m_initialName);
}

const std::string& RenameObjectCommand::typeName()
{
	static const std::string name = "RenameObjectCommand";
	return name;
}

const std::string& RenameObjectCommand::description()
{
	return m_description;
}
