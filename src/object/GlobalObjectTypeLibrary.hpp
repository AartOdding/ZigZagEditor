#pragma once

#include <memory>
#include <unordered_map>

#include <object/ObjectType.hpp>



class GlobalObjectTypeLibrary
{
public:

	GlobalObjectTypeLibrary(GlobalObjectTypeLibrary&&) = delete;
	GlobalObjectTypeLibrary(const GlobalObjectTypeLibrary&) = delete;

	static GlobalObjectTypeLibrary* getInstance();

	void addNewObjectType(std::string_view name, Identifier identifier, ObjectTypeCategory category);


private:

	GlobalObjectTypeLibrary() = default;

	std::unordered_map<Identifier, std::unique_ptr<ObjectType>> m_ownedObjectTypes;


};
