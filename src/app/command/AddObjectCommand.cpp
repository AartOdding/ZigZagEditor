#include <stdexcept>

#include <app/command/AddObjectCommand.hpp>
#include <interop/ObjectInterop.hpp>



AddObjectCommand::AddObjectCommand(Identifier<ObjectType> typeID, Identifier<ZObject> parentID)
	: m_parentID(parentID)
	, m_ownsObject(false)
{
	assert(IdentityMap<ObjectType>::get(typeID));
	assert(IdentityMap<ZObject>::get(parentID));

	// TODO LOGGING: log error if object type does not exist, or parent is null.

	if (IdentityMap<ObjectType>::get(typeID))
	{
		// Generate the object without a parent first, set parent in redo
		m_objectID = addObject(typeID, Identifier<ZObject>());
		m_ownsObject = static_cast<bool>(m_objectID);
	}
	// else log error

	m_description = "Added object <" + std::to_string(static_cast<std::uint64_t>(m_objectID)) + ">";
}

AddObjectCommand::~AddObjectCommand()
{
	if (m_ownsObject)
	{
		removeObject(m_objectID);
	}
}

bool AddObjectCommand::redo()
{
	if (m_objectID && m_parentID)
	{
		if (setObjectParent(m_objectID, m_parentID))
		{
			m_ownsObject = false;
			return true;
		}
	}
	// TODO LOGGING: log info that a command was unsuccesful
	return false;
}

bool AddObjectCommand::undo()
{
	if (setObjectParent(m_objectID, Identifier<ZObject>()))
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
