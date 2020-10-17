#pragma once

#include <cstdint>

#include <object/ObjectType.hpp>
#include <util/Platform.hpp>


class ZObject;
class ObjectType;


bool addObject(Identifier<ObjectType> objectType, Identifier<ZObject> parentObject);
bool removeObject(Identifier<ZObject> object);


typedef bool (*AddObjectDelegate)(std::uint64_t objectType, std::uint64_t parentObject);
typedef bool (*RemoveObjectDelegate)(std::uint64_t object);

ZIGZAG_API void installObjectDelegates(AddObjectDelegate add, RemoveObjectDelegate rm);


ZIGZAG_API void onNewObjectTypeAdded(const char* name, std::uint64_t uniqueID, ObjectTypeCategory category);


ZIGZAG_API void onObjectCreated(std::uint64_t newObject, std::uint64_t parentObject, std::uint64_t objectType);

ZIGZAG_API void onObjectDestroyed(std::uint64_t objectID);

