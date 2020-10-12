#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include <object/Identity.hpp>


class ObjectTypeNamespace;

enum class ObjectTypeCategory
{
    Object,
    DataSource,
    Operator
};

class ObjectType : public Identity<ObjectType>
{
    ObjectType() = delete;
    ObjectType(ObjectType&&) = delete;
    ObjectType(const ObjectType&) = delete;

public:

    using Pointer = std::unique_ptr<ObjectType>;

    static Pointer create(Identifier<ObjectType> identifier);
    ~ObjectType() = default;

    const std::string& getName() const;
    void setName(std::string_view name);

    ObjectTypeCategory getCategory() const;
    void setCategory(ObjectTypeCategory category);

    ObjectTypeNamespace* getNamespace();
    const ObjectTypeNamespace* getNamespace() const;

private:

    friend class ObjectTypeNamespace;

    ObjectType(Identifier<ObjectType> identifier);

    std::string m_name;
    ObjectTypeCategory m_category;
    ObjectTypeNamespace* m_namespace;

};
