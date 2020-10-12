#include <object/ObjectTypeNamespace.hpp>


ObjectTypeNamespace::ObjectTypeNamespace(std::string_view name)
    : Identity<ObjectTypeNamespace>(this)
    , m_name(name)
{
}

ObjectTypeNamespace::Pointer ObjectTypeNamespace::create(std::string_view name)
{
    return std::unique_ptr<ObjectTypeNamespace>(new ObjectTypeNamespace(name));
}

void ObjectTypeNamespace::addChild(Pointer&& childNamespace)
{
    childNamespace->m_parent = this;
    m_children.push_back(std::move(childNamespace));
}

ObjectTypeNamespace::Pointer ObjectTypeNamespace::removeChild(Identifier<ObjectTypeNamespace> childIdentifier)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->getIdentifier() == childIdentifier)
        {
            Pointer child = std::move(*it);
            child->m_parent = nullptr;
            m_children.erase(it);
            return std::move(child);
        }
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

void ObjectTypeNamespace::addType(TypePointer&& type)
{
    type->m_namespace = this;
    m_types.push_back(std::move(type));
}

ObjectTypeNamespace::TypePointer ObjectTypeNamespace::removeType(Identifier<ObjectType> typeIdentifier)
{
    for(auto it = m_types.begin(); it != m_types.end(); ++it)
    {
        if ((*it)->getIdentifier() == typeIdentifier)
        {
            TypePointer type = std::move(*it);
            type->m_namespace = nullptr;
            m_types.erase(it);
            return std::move(type);
        }
    }
    return TypePointer();
}

ObjectTypeNamespace::TypesView ObjectTypeNamespace::getTypes()
{
    return TypesView(m_types);
}

ObjectTypeNamespace::ConstTypesView ObjectTypeNamespace::getTypes() const
{
    return ConstTypesView(m_types);
}

const std::string& ObjectTypeNamespace::getName() const
{
    return m_name;
}
