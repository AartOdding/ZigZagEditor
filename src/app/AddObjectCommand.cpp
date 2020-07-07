#include "AddObjectCommand.hpp"

#include <stdexcept>

#include <ZigZag/ObjectFactory.hpp>


using namespace ZigZag;


AddObjectCommand::AddObjectCommand(ZigZag::Object* parentObject, const std::string& typeName)
	: m_parentObject(parentObject)
{
	if (!parentObject)
	{
		throw std::runtime_error("Cannot add object to nullptr.");
	}
	m_addedObject = ObjectFactory::instance()->create(typeName);
	m_addedObject->setDeleteByParent(false);
	m_ownsObject = true;
	m_addedObject->setName(m_addedObject->typeName());

	if (!m_addedObject)
	{
		throw std::runtime_error("Object type does not exist.");
	}
}

AddObjectCommand::~AddObjectCommand()
{
	if (m_addedObject && m_ownsObject)
	{
		delete m_addedObject;
	}
}

void AddObjectCommand::redo()
{
	m_addedObject->setParent(m_parentObject);
	m_addedObject->setDeleteByParent(true);
	m_ownsObject = false;
	m_description = "Add " + m_addedObject->getName() + " to " + m_parentObject->getName();
}

void AddObjectCommand::undo()
{
	m_addedObject->setDeleteByParent(false);
	m_ownsObject = true;
	m_addedObject->setParent(nullptr);
}

const std::string& AddObjectCommand::typeName()
{
	static const std::string name = "AddObjectCommand";
	return name;
}

const std::string& AddObjectCommand::getDescription()
{
	return m_description;
}
