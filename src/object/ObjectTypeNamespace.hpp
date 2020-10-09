#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <object/ObjectType.hpp>
#include <tci/views.hpp>


class ObjectTypeNamespace
{
    ObjectTypeNamespace() = delete;
    ObjectTypeNamespace(ObjectTypeNamespace&&) = delete;
    ObjectTypeNamespace(const ObjectTypeNamespace&) = delete;

public:
    
    using Pointer = std::unique_ptr<ObjectTypeNamespace>;

    using Children = std::vector<std::unique_ptr<ObjectTypeNamespace>>;
    using ChildrenView = tci::pointer_view<Children>;
    using ConstChildrenView = tci::const_pointer_view<Children>;

    using Types = std::vector<std::unique_ptr<ObjectType>>;
    using TypesView = tci::pointer_view<Types>;
    using ConstTypesView = tci::pointer_view<Types>;


    static Pointer create(std::string_view name);
    ~ObjectTypeNamespace();


    void addChild(Pointer&& childNamespace);
    Pointer removeFromParent();


    ObjectTypeNamespace* getParent();
    const ObjectTypeNamespace* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;


    TypesView getTypes();
    ConstTypesView getTypes() const;


    const std::string& getName() const;

private:

    ObjectTypeNamespace(std::string_view name);
    
    Types m_types;

    Children m_children;
    ObjectTypeNamespace* m_parent;

    std::string m_name;

};
