#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <object/Identity.hpp>
#include <object/ObjectType.hpp>
#include <tci/views.hpp>


class ObjectTypeNamespace : public Identity<ObjectTypeNamespace>
{
    ObjectTypeNamespace() = delete;
    ObjectTypeNamespace(ObjectTypeNamespace&&) = delete;
    ObjectTypeNamespace(const ObjectTypeNamespace&) = delete;

public:
    
    using Pointer = std::unique_ptr<ObjectTypeNamespace>;
    using TypePointer = std::unique_ptr<ObjectType>;

    using Children = std::vector<Pointer>;
    using ChildrenView = tci::pointer_view<Children>;
    using ConstChildrenView = tci::const_pointer_view<Children>;

    using Types = std::vector<TypePointer>;
    using TypesView = tci::pointer_view<Types>;
    using ConstTypesView = tci::pointer_view<Types>;


    static Pointer create(std::string_view name);
    ~ObjectTypeNamespace() = default;

    void addChild(Pointer&& childNamespace);
    Pointer removeChild(Identifier<ObjectTypeNamespace> childNamespace);

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

    std::string m_name;

};
