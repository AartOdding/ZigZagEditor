#include <object/GlobalObjectMap.hpp>


GlobalObjectMap* GlobalObjectMap::getInstance()
{
    GlobalObjectMap instance;
    return &instance;
}

ZObject* GlobalObjectMap::get(Identifier identifier) const
{
    auto it = m_objects.find(identifier);

    if (it != m_objects.end())
    {
        return it->second;
    }
    return nullptr;
}

ZObject* GlobalObjectMap::operator[](Identifier identifier) const
{
    return get(identifier);
}
