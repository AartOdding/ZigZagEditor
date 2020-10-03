#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <object/Identifier.hpp>


enum class ObjectTypeCategory
{
    DataSource,
    Operator,
    Object
};


class ObjectType
{
public:

    ObjectType(std::string_view name, Identifier uniqueID, ObjectTypeCategory category);

    const std::string& getName() const;
    Identifier getUniqueID() const;
    ObjectTypeCategory getCategory() const;

private:

    std::string m_name;
    Identifier m_uniqueID;
    ObjectTypeCategory m_category;

};
