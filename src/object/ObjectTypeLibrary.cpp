#include <object/ObjectTypeLibrary.hpp>



ObjectTypeLibrary* ObjectTypeLibrary::getGLobalInstance()
{
	static ObjectTypeLibrary globalInstance;
	return &globalInstance;
}

void ObjectTypeLibrary::addNewObjectType(std::string_view name, Identifier uniqueID, ObjectTypeCategory category)
{
	m_ownedObjectTypes[uniqueID] = std::make_unique<ObjectType>(name, uniqueID, category);
}
