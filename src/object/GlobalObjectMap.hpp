#pragma once

#include <unordered_map>

#include <object/Identifier.hpp>
#include <object/ZObject.hpp>


class GlobalObjectMap
{
public:

	GlobalObjectMap(GlobalObjectMap&&) = delete;
	GlobalObjectMap(const GlobalObjectMap&) = delete;

	static GlobalObjectMap* getInstance();

    ZObject* get(Identifier identifier) const;
    ZObject* operator[](Identifier identifier) const;

private:

    friend class ZObject;

    GlobalObjectMap() = default;

    std::unordered_map<Identifier, ZObject*> m_objects;

};
