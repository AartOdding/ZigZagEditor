#include <object/GlobalObjectTypeLibrary.hpp>



GlobalObjectTypeLibrary* GlobalObjectTypeLibrary::getInstance()
{
	static GlobalObjectTypeLibrary instance;
	return &instance;
}

void GlobalObjectTypeLibrary::addNewObjectType(std::string_view name, Identifier identifier, ObjectTypeCategory category)
{
	m_ownedObjectTypes[identifier] = std::make_unique<ObjectType>(name, identifier, category);
}
