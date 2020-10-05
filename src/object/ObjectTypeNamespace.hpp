#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <object/ObjectType.hpp>


class ObjectTypeNamespace
{
public:

    ObjectTypeNamespace() = delete;
    ObjectTypeNamespace(ObjectTypeNamespace&&) = delete;
    ObjectTypeNamespace(const ObjectTypeNamespace&) = delete;

    ~ObjectTypeNamespace();

    static std::unique_ptr<ObjectTypeNamespace> createNamespace(std::string_view name);

    ObjectTypeNamespace* createChildNamespace(std::string_view name);
    void addChildNamespace(std::unique_ptr<ObjectTypeNamespace>&& childNamespace);
    std::unique_ptr<ObjectTypeNamespace> stealFromParent();

    ObjectTypeNamespace* getParentNamespace();
    const std::vector<std::unique_ptr<ObjectTypeNamespace>>& getChildNamespaces();

private:

    ObjectTypeNamespace(std::string_view name);
    
    std::vector<std::unique_ptr<ObjectType>> m_objectTypes;
    std::vector<std::unique_ptr<ObjectTypeNamespace>> m_childNamespaces;

    ObjectTypeNamespace* m_parentNamespace;
    std::string m_name;

};
