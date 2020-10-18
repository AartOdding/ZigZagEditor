#pragma once

#include <cstdint>

#include <object/ObjectType.hpp>
#include <util/Platform.hpp>


class ZObject;
class ObjectType;


Identifier<ZObject> addObject(Identifier<ObjectType> objectType, Identifier<ZObject> parentObject);
bool removeObject(Identifier<ZObject> object);
bool setObjectParent(Identifier<ZObject> object, Identifier<ZObject> newParent);


typedef std::uint64_t(*AddObjectDelegate)(std::uint64_t typeID, std::uint64_t parentID);
typedef bool (*RemoveObjectDelegate)(std::uint64_t objectID);
typedef bool (*SetObjectParentDelegate)(std::uint64_t objectID, std::uint64_t newParentID);

ZIGZAG_API void installObjectDelegates(AddObjectDelegate addDelegate, RemoveObjectDelegate removeDelegate, 
	SetObjectParentDelegate reparentDelegate);



ZIGZAG_API void onNewObjectTypeAdded(const char* name, std::uint64_t typeID, ObjectTypeCategory category);

ZIGZAG_API void onObjectCreated(std::uint64_t newObjectID, std::uint64_t parentID, std::uint64_t typeID);
ZIGZAG_API void onObjectDestroyed(std::uint64_t objectID);
ZIGZAG_API void onObjectReparented(std::uint64_t objectID, std::uint64_t newParentID);
