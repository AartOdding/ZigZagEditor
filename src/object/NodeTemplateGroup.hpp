#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <object/Identity.hpp>
#include <object/NodeTemplate.hpp>
#include <tci/views.hpp>



class NodeTemplateGroup
{
    NodeTemplateGroup() = delete;
    NodeTemplateGroup(NodeTemplateGroup&&) = delete;
    NodeTemplateGroup(const NodeTemplateGroup&) = delete;

public:
    
    using Pointer = std::unique_ptr<NodeTemplateGroup>;
    using TypePointer = std::unique_ptr<NodeTemplate>;

    using Children = std::map<std::string, Pointer>;
    using ChildrenView = tci::values_view<Children>;
    using ConstChildrenView = tci::const_values_view<Children>;

    using Types = std::unordered_map<Identifier<NodeTemplate>, TypePointer>;
    using TypesAlphabetic = std::multimap<std::string, NodeTemplate*>;
    using TypesView = tci::values_view<TypesAlphabetic>;
    using ConstTypesView = tci::const_values_view<TypesAlphabetic>;


    static Pointer create(std::string_view name);
    ~NodeTemplateGroup() = default;

    NodeTemplateGroup* addChild(Pointer&& childNamespace);
    NodeTemplateGroup* getChild(std::string_view childNamespace);
    Pointer removeChild(std::string_view childNamespace);

    NodeTemplateGroup* getParent();
    const NodeTemplateGroup* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;

    NodeTemplate* addType(TypePointer&& type);
    TypePointer removeType(Identifier<NodeTemplate> type);

    TypesView getTypes();
    ConstTypesView getTypes() const;

    const std::string& getName() const;

private:

    NodeTemplateGroup(std::string_view name);
    
    Children m_children;
    NodeTemplateGroup* m_parent = nullptr;

    Types m_types;
    TypesAlphabetic m_typesAlphabetic;

    const std::string m_name;

};
