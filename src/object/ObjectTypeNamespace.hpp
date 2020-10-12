#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <object/Identity.hpp>
#include <object/ObjectType.hpp>
#include <tci/views.hpp>


class ObjectTypeNamespace
{
    ObjectTypeNamespace() = delete;
    ObjectTypeNamespace(ObjectTypeNamespace&&) = delete;
    ObjectTypeNamespace(const ObjectTypeNamespace&) = delete;

public:
    
    using Pointer = std::unique_ptr<ObjectTypeNamespace>;
    using TypePointer = std::unique_ptr<ObjectType>;

    using Children = std::map<std::string, Pointer>;
    using ChildrenView = tci::values_view<Children>;
    using ConstChildrenView = tci::const_values_view<Children>;

    using Types = std::unordered_map<Identifier<ObjectType>, TypePointer>;
    using TypesAlphabetic = std::multimap<std::string, ObjectType*>;
    using TypesView = tci::values_view<TypesAlphabetic>;
    using ConstTypesView = tci::const_values_view<TypesAlphabetic>;


    static Pointer create(std::string_view name);
    ~ObjectTypeNamespace() = default;

    void addChild(Pointer&& childNamespace);
    Pointer removeChild(std::string_view childNamespace);

    ObjectTypeNamespace* getParent();
    const ObjectTypeNamespace* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;

    void addType(TypePointer&& type);
    TypePointer removeType(Identifier<ObjectType> type);

    TypesView getTypes();
    ConstTypesView getTypes() const;

    const std::string& getName() const;

private:

    ObjectTypeNamespace(std::string_view name);
    
    Children m_children;
    ObjectTypeNamespace* m_parent;

    Types m_types;
    TypesAlphabetic m_typesAlphabetic;

    const std::string m_name;

};
