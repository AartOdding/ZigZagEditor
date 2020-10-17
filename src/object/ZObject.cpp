#include <stdexcept>

#include <object/ZObject.hpp>


ZObject::ZObject(Identifier<ZObject> identifier)
    : Identity<ZObject>(this, identifier)
    , m_parent(nullptr)
{
}

std::unique_ptr<ZObject> ZObject::create(Identifier<ZObject> identifier)
{
    if (identifier)
    {
        return std::unique_ptr<ZObject>(new ZObject(identifier));
    }
    return std::unique_ptr<ZObject>();
}

ZObject* ZObject::addChild(Pointer&& child)
{
    if (child && !child->m_parent)
    {
        child->m_parent = this;
        m_children.push_back(std::move(child));
        return m_children.back().get();
    }
    return nullptr;
}

ZObject::Pointer ZObject::removeChild(Identifier<ZObject> childIdentifier)
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

ZObject* ZObject::getParent()
{
    return m_parent;
}

const ZObject* ZObject::getParent() const
{
    return m_parent;
}

ZObject::ChildrenView ZObject::getChildren()
{
    return ChildrenView(m_children);
}

ZObject::ConstChildrenView ZObject::getChildren() const
{
    return ConstChildrenView(m_children);
}

void ZObject::setNodeCategory(ObjectTypeCategory category)
{
    m_category = category;
}

ObjectTypeCategory ZObject::getNodeCategory() const
{
    return m_category;
}

void ZObject::setName(std::string_view name)
{
    m_name = name;
}

const std::string& ZObject::getName() const
{
    return m_name;
}
