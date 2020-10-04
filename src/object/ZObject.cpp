#include <stdexcept>

#include <object/ZObject.hpp>
#include <object/GlobalObjectMap.hpp>


ZObject::ZObject(Identifier identifier)
    : m_identifier(identifier)
    , m_parent(nullptr)
{
    if (identifier)
    {
        GlobalObjectMap::getInstance()->m_objects[identifier] = this;
    }
}

ZObject::~ZObject()
{
    if (m_identifier)
    {
        GlobalObjectMap::getInstance()->m_objects.erase(m_identifier);
    }
}

std::unique_ptr<ZObject> ZObject::create(Identifier identifier)
{
    if (identifier)
    {
        return std::unique_ptr<ZObject>(new ZObject(identifier));
    }
    return std::unique_ptr<ZObject>();
}

ZObject* ZObject::createChild(Identifier identifier)
{
    if (identifier)
    {
        auto newObject = m_children.emplace_back(new ZObject(identifier)).get();
        newObject->m_parent = this;
        return newObject;
    }
    return nullptr;
}

Identifier ZObject::getIdentifier() const
{
    return m_identifier;
}

ZObject* ZObject::getParent()
{
    return m_parent;
}

const std::vector<std::unique_ptr<ZObject>>& ZObject::getChildren()
{
    return m_children;
}

std::unique_ptr<ZObject> ZObject::stealFromParent()
{
    if (m_parent)
    {
        for (auto it = m_parent->m_children.begin(); it != m_parent->m_children.end(); ++it)
        {
            if (this == it->get())
            {
                std::unique_ptr<ZObject> uniqueThis = std::move(*it);
                m_parent->m_children.erase(it);
                m_parent = nullptr;
                return std::move(uniqueThis);
            }
        }
    }
    return std::unique_ptr<ZObject>();
}

void ZObject::addChild(std::unique_ptr<ZObject>&& child)
{
    if (child && !child->m_parent)
    {
        child->m_parent = this;
        m_children.push_back(std::move(child));
    }
}
