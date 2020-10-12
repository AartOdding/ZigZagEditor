#include <Platform.hpp>
#include <interop/ObjectInterop.hpp>
#include <object/ZObject.hpp>
#include <object/ObjectType.hpp>
#include <object/ObjectTypeNamespace.hpp>
#include <util/StringUtils.hpp>

#include <iostream>


typedef bool (*AddObjectDelegate)(std::uint64_t objectType, std::uint64_t parentObject);
typedef bool (*RemoveObjectDelegate)(std::uint64_t object);

namespace
{
    AddObjectDelegate addObjectDelegate = nullptr;
    RemoveObjectDelegate removeObjectDelegate = nullptr;
}


bool addObject(ObjectType* type, ZObject* object)
{
    if (type && object && addObjectDelegate)
    {
        return addObjectDelegate(static_cast<std::uint64_t>(type->getIdentifier()),
                                 static_cast<std::uint64_t>(object->getIdentifier()));
    }
    return false;
}

bool removeObject(ZObject* object)
{
    if (object && removeObjectDelegate)
    {
        return removeObjectDelegate(static_cast<std::uint64_t>(object->getIdentifier()));
    }
    return false;
}

ZIGZAG_EXPORT void installObjectDelegates(AddObjectDelegate add, RemoveObjectDelegate rm)
{
    addObjectDelegate = add;
    removeObjectDelegate = rm;
}

ZIGZAG_EXPORT void onNewObjectTypeAdded(const char* name, std::uint64_t uniqueID, ObjectTypeCategory category)
{
    auto identifier = Identifier<ObjectType>(uniqueID);
    auto nameParts = StringUtils::split(name, '.');

    if (identifier && !nameparts.empty())
    {
        auto objectType = ObjectType::create(identifier);
        objectType->setName(nameParts.back());
        objectType->setCategory(category);

        for (auto it = nameParts.rbegin() + 1; it != nameParts.rend() ++it)
        {
            ObjectTypeNamespace::crea
        }
    }
    auto objectType = ObjectType::create(Identifier<ObjectType>(uniqueID));


    
    auto IdentityMap<ObjectType>::get(Identifier<ObjectType>(uniqueID));
    GlobalObjectTypeLibrary::getInstance()->addNewObjectType(name, Identifier(uniqueID), category);
    std::cout << "Object type was added: " << name << ", id: " << uniqueID << ", category: " << static_cast<int>(category) << std::endl;
}

ZIGZAG_EXPORT void onObjectCreated(std::uint64_t newObject, std::uint64_t parentObject)
{
    auto parent = GlobalObjectMap::getInstance()->get(Identifier(parentObject));
    parent->createChild(Identifier(newObject));
}

ZIGZAG_EXPORT void onObjectDestroyed(std::uint64_t objectID)
{
    auto object = GlobalObjectMap::getInstance()->get(Identifier(objectID));
    object->stealFromParent(); // not storing the returned unique_ptr will delete it.
}
