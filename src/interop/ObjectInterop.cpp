#include <Application.hpp>
#include <interop/ObjectInterop.hpp>
#include <object/ZObject.hpp>
#include <object/ObjectTypeNamespace.hpp>
#include <util/StringUtils.hpp>

#include <iostream>



namespace
{
    AddObjectDelegate addObjectDelegate = nullptr;
    RemoveObjectDelegate removeObjectDelegate = nullptr;
    SetObjectParentDelegate setObjectParentDelegate = nullptr;

    std::unordered_map<Identifier<ZObject>, ZObject::Pointer> parentlessObjects;
}


Identifier<ZObject> addObject(Identifier<ObjectType> objectType, Identifier<ZObject> parentObject)
{
    if (objectType && addObjectDelegate)
    {
        return Identifier<ZObject>(addObjectDelegate(static_cast<std::uint64_t>(objectType),
                                                     static_cast<std::uint64_t>(parentObject)));
    }
    return Identifier<ZObject>();
}

bool removeObject(Identifier<ZObject> object)
{
    if (object && removeObjectDelegate)
    {
        return removeObjectDelegate(static_cast<std::uint64_t>(object));
    }
    return false;
}

bool setObjectParent(Identifier<ZObject> object, Identifier<ZObject> newParent)
{
    if (object && setObjectParentDelegate)
    {
        return setObjectParentDelegate(static_cast<std::uint64_t>(object),
                                       static_cast<std::uint64_t>(newParent));
    }
    return false;
}

ZIGZAG_API void installObjectDelegates(AddObjectDelegate addDelegate, 
    RemoveObjectDelegate removeDelegate, SetObjectParentDelegate parentDelegate)
{
    std::cout << "[editor dll] installing object delegates" << std::endl;
    addObjectDelegate = addDelegate;
    removeObjectDelegate = removeDelegate;
    setObjectParentDelegate = parentDelegate;
}

ZIGZAG_API void onNewObjectTypeAdded(const char* name, std::uint64_t uniqueID, ObjectTypeCategory category)
{
    std::cout << "[editor dll] object type added: " << name << " " << uniqueID << std::endl;
    auto identifier = Identifier<ObjectType>(uniqueID);
    auto nameParts = StringUtils::split(name, '.');
    
    if (identifier && !nameParts.empty())
    {
        auto objectType = ObjectType::create(nameParts.back(), identifier);
        objectType->setCategory(category);
        nameParts.pop_back();

        ObjectTypeNamespace* typeNamespace = Application::getGlobalInstance()->getRootTypeNamespace();

        for (const auto& namespaceName : nameParts)
        {
            auto childNamespace = typeNamespace->getChild(namespaceName);

            if (childNamespace)
            {
                typeNamespace = childNamespace;
            }
            else
            {
                typeNamespace = typeNamespace->addChild(ObjectTypeNamespace::create(namespaceName));
            }
        }

        typeNamespace->addType(std::move(objectType));
    }
}

ZIGZAG_API void onObjectCreated(std::uint64_t newObject, std::uint64_t parentObject, std::uint64_t objectType)
{
    std::cout << "[editor dll] object created: " << newObject << std::endl;

    Identifier<ZObject> objectID(newObject);
    Identifier<ZObject> parentID(parentObject);
    Identifier<ObjectType> typeID(objectType);

    auto parent = const_cast<ZObject*>(IdentityMap<ZObject>::get(parentID));
    auto type = IdentityMap<ObjectType>::get(typeID);

    auto object = ZObject::create(objectID);

    if (type)
    {
        object->setName(type->getName());
        object->setNodeCategory(type->getCategory());

        if (parent)
        {
            parent->addChild(std::move(object));
        }
        else
        {
            parentlessObjects[object->getIdentifier()] = std::move(std::move(object));
        }
    }
}

ZIGZAG_API void onObjectDestroyed(std::uint64_t objectID_)
{
    auto objectID = Identifier<ZObject>(objectID_);

    std::cout << "[editor dll] object destroyed: " << objectID << std::endl;

    auto objectConst = IdentityMap<ZObject>::get(Identifier<ZObject>(objectID));
    auto object = const_cast<ZObject*>(objectConst);

    if (object && object->getParent())
    {
        object->getParent()->removeChild(Identifier<ZObject>(objectID));
    }
    else if (object && parentlessObjects.count(objectID) == 1)
    {
        parentlessObjects.erase(objectID);
    }
}

ZIGZAG_API void onObjectReparented(std::uint64_t objectID_, std::uint64_t newParentID_)
{
    auto objectID = Identifier<ZObject>(objectID_);
    auto newParentID = Identifier<ZObject>(newParentID_);

    auto objectPtr = const_cast<ZObject*>(IdentityMap<ZObject>::get(objectID));
    auto newParentPtr = const_cast<ZObject*>(IdentityMap<ZObject>::get(newParentID));
    auto oldParentPtr = objectPtr->getParent();

    if (objectPtr)
    {
        ZObject::Pointer object;

        if (objectPtr->getParent())
        {
            object = objectPtr->getParent()->removeChild(objectID);
        }
        else
        {
            object = std::move(parentlessObjects[objectID]);
            parentlessObjects.erase(objectID);
        }
        if (object)
        {
            if (newParentPtr)
            {
                newParentPtr->addChild(std::move(object));
            }
            else
            {
                parentlessObjects[object->getIdentifier()] = std::move(object);
            }
        }
    }

    std::cout << "[editor dll] object reparented: " << objectID << std::endl;

}
