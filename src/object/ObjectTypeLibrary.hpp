#pragma once

#include <memory>
#include <unordered_map>

#include <object/ObjectType.hpp>



class ObjectTypeLibrary
{
public:

	ObjectTypeLibrary(ObjectTypeLibrary&&) = delete;
	ObjectTypeLibrary(const ObjectTypeLibrary&) = delete;

	static ObjectTypeLibrary* getGlobalInstance();

	void addNewObjectType(std::string_view name, Identifier uniqueID, ObjectTypeCategory category);


private:

	ObjectTypeLibrary() = default;

	std::unordered_map<Identifier, std::unique_ptr<ObjectType>> m_ownedObjectTypes;


};
