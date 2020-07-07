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
	m_object->setDeleteByParent(false);
	m_ownsObject = true;
	m_object->setParent(nullptr);
}

void RemoveObjectCommand::undo()
{
	m_object->setParent(m_parentObject);
	m_object->setDeleteByParent(true);
	m_ownsObject = false;
}

const std::string& RemoveObjectCommand::typeName()
{
	static const std::string name = "RemoveObjectCommand";
	return name;
}

const std::string& RemoveObjectCommand::getDescription()
{
	return m_description;
}
