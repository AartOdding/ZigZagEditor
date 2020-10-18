#include "AddObjectCommand.hpp"

#include <stdexcept>
#include <interop/ObjectInterop.hpp>



AddObjectCommand::AddObjectCommand(Identifier<ObjectType> objectType, Identifier<ZObject> parentObject)
	: m_parentObject(parentObject)
{
	m_object = addObject(objectType, Identifier<ZObject>());
	m_ownsObject = true;
	m_description = "Added object " + std::to_string(m_object);
}

AddObjectCommand::~AddObjectCommand()
{
	if (m_ownsObject)
	{
		removeObject(m_object);
	}
}

bool AddObjectCommand::redo()
{
	if (setObjectParent(m_object, m_parentObject))
	{
		m_ownsObject = false;
		return true;
	}
	return false;
}

bool AddObjectCommand::undo()
{
	if (setObjectParent(m_object, Identifier<ZObject>()))
	{
		m_ownsObject = true;
		return true;
	}
	return false;
}

const std::string& AddObjectCommand::getCommandName()
{
	static const std::string name = "AddObjectCommand";
	return name;
}

const std::string& AddObjectCommand::description()
{
	return m_description;
}
