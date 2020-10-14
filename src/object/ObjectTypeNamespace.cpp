#include <algorithm>
#include <cassert>

#include <object/ObjectTypeNamespace.hpp>


ObjectTypeNamespace::ObjectTypeNamespace(std::string_view name)
    : m_name(name)
{
}

ObjectTypeNamespace::Pointer ObjectTypeNamespace::create(std::string_view name)
{
    return std::unique_ptr<ObjectTypeNamespace>(new ObjectTypeNamespace(name));
}

ObjectTypeNamespace* ObjectTypeNamespace::addChild(Pointer&& childNamespace)
{
    assert(m_children.count(childNamespace->getName()) == 0);

    if (childNamespace)
    {
        auto returnValue = childNamespace.get();
        childNamespace->m_parent = this;
        m_children[childNamespace->getName()] = std::move(childNamespace);
        return returnValue;
    }
    return nullptr;
}

ObjectTypeNamespace* ObjectTypeNamespace::getChild(std::string_view childNamespace)
{
    auto it = std::lower_bound(m_children.begin(), m_children.end(), childNamespace,
        [](const auto& pair, const std::string_view& name) { return pair.first < name; });

    if (it != m_children.end() && it->first == childNamespace)
    {
        return it->second.get();
    }
    return nullptr;
}

ObjectTypeNamespace::Pointer ObjectTypeNamespace::removeChild(std::string_view childNamespace)
{
    auto it = std::lower_bound(m_children.begin(), m_children.end(), childNamespace,
        [](const auto& pair, const std::string_view& name){ return pair.first < name; });

    if (it != m_children.end() && it->first == childNamespace)
    {
        Pointer child = std::move(it->second);
        child->m_parent = nullptr;
        m_children.erase(it);
        return std::move(child);
    }
    return Pointer();
}

ObjectTypeNamespace* ObjectTypeNamespace::getParent()
{
    return m_parent;
}

const ObjectTypeNamespace* ObjectTypeNamespace::getParent() const
{
    return m_parent;
}

ObjectTypeNamespace::ChildrenView ObjectTypeNamespace::getChildren()
{
    return ChildrenView(m_children);
}

ObjectTypeNamespace::ConstChildrenView ObjectTypeNamespace::getChildren() const
{
    return ConstChildrenView(m_children);
}

ObjectType* ObjectTypeNamespace::addType(TypePointer&& type)
{
    if (type)
    {
        auto returnValue = type.get();
        type->m_namespace = this;
        m_typesAlphabetic.emplace(type->getName(), type.get());
        m_types[type->getIdentifier()] = std::move(type);
        return returnValue;
    }
    return nullptr;
}

ObjectTypeNamespace::TypePointer ObjectTypeNamespace::removeType(Identifier<ObjectType> typeIdentifier)
{
    if (typeIdentifier)
    {
        auto typeIt = m_types.find(typeIdentifier);

        if (typeIt != m_types.end())
        {
            auto [begin, end] = m_typesAlphabetic.equal_range(typeIt->second->getName());
            
            for (auto it = begin; it != end; ++it)
            {
                if (it->second == typeIt->second.get())
                {
                    m_typesAlphabetic.erase(it);
                    break;
                }
            }
            TypePointer type = std::move(typeIt->second);
            type->m_namespace = nullptr;
            m_types.erase(typeIt);
            return std::move(type);
        }
    }
    return TypePointer();
}

ObjectTypeNamespace::TypesView ObjectTypeNamespace::getTypes()
{
    return TypesView(m_typesAlphabetic);
}

ObjectTypeNamespace::ConstTypesView ObjectTypeNamespace::getTypes() const
{
    return ConstTypesView(m_typesAlphabetic);
}

const std::string& ObjectTypeNamespace::getName() const
{
    return m_name;
}
