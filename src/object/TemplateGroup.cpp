#include <algorithm>
#include <cassert>

#include <object/TemplateGroup.hpp>


TemplateGroup::TemplateGroup(std::string_view name)
    : m_name(name)
{
}

TemplateGroup::Pointer TemplateGroup::create(std::string_view name)
{
    return std::unique_ptr<TemplateGroup>(new TemplateGroup(name));
}

TemplateGroup* TemplateGroup::addChild(Pointer&& childNamespace)
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

TemplateGroup* TemplateGroup::getChild(std::string_view childNamespace)
{
    auto it = std::lower_bound(m_children.begin(), m_children.end(), childNamespace,
        [](const auto& pair, const std::string_view& name) { return pair.first < name; });

    if (it != m_children.end() && it->first == childNamespace)
    {
        return it->second.get();
    }
    return nullptr;
}

TemplateGroup::Pointer TemplateGroup::removeChild(std::string_view childNamespace)
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

TemplateGroup* TemplateGroup::getParent()
{
    return m_parent;
}

const TemplateGroup* TemplateGroup::getParent() const
{
    return m_parent;
}

TemplateGroup::ChildrenView TemplateGroup::getChildren()
{
    return ChildrenView(m_children);
}

TemplateGroup::ConstChildrenView TemplateGroup::getChildren() const
{
    return ConstChildrenView(m_children);
}

NodeTemplate* TemplateGroup::addType(TypePointer&& type)
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

TemplateGroup::TypePointer TemplateGroup::removeType(Identifier<NodeTemplate> typeIdentifier)
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

TemplateGroup::TypesView TemplateGroup::getTypes()
{
    return TypesView(m_typesAlphabetic);
}

TemplateGroup::ConstTypesView TemplateGroup::getTypes() const
{
    return ConstTypesView(m_typesAlphabetic);
}

const std::string& TemplateGroup::getName() const
{
    return m_name;
}
