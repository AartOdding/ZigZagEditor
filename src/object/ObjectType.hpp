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
    ObjectType() = delete;
    ObjectType(ObjectType&&) = delete;
    ObjectType(const ObjectType&) = delete;

public:

    using Pointer = std::unique_ptr<ObjectType>;

    static Pointer create(Identifier identifier);
    ~ObjectType();


 
    const std::string& getName() const;
    Identifier getIdentifier() const;
    ObjectTypeCategory getCategory() const;

private:

    ObjectType(std::string_view name, Identifier identifier, ObjectTypeCategory category);

    std::string m_name;
    Identifier m_identifier;
    ObjectTypeCategory m_category;

};
