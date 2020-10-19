#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <object/Identity.hpp>
#include <object/Template.hpp>
#include <tci/views.hpp>



class TemplateGroup
{
    TemplateGroup() = delete;
    TemplateGroup(TemplateGroup&&) = delete;
    TemplateGroup(const TemplateGroup&) = delete;

public:
    
    using Pointer = std::unique_ptr<TemplateGroup>;
    using TypePointer = std::unique_ptr<Template>;

    using Children = std::map<std::string, Pointer>;
    using ChildrenView = tci::values_view<Children>;
    using ConstChildrenView = tci::const_values_view<Children>;

    using Types = std::unordered_map<Identifier<Template>, TypePointer>;
    using TypesAlphabetic = std::multimap<std::string, Template*>;
    using TypesView = tci::values_view<TypesAlphabetic>;
    using ConstTypesView = tci::const_values_view<TypesAlphabetic>;


    static Pointer create(std::string_view name);
    ~TemplateGroup() = default;

    TemplateGroup* addChild(Pointer&& childNamespace);
    TemplateGroup* getChild(std::string_view childNamespace);
    Pointer removeChild(std::string_view childNamespace);

    TemplateGroup* getParent();
    const TemplateGroup* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;

    Template* addType(TypePointer&& type);
    TypePointer removeType(Identifier<Template> type);

    TypesView getTypes();
    ConstTypesView getTypes() const;

    const std::string& getName() const;

private:

    TemplateGroup(std::string_view name);
    
    Children m_children;
    TemplateGroup* m_parent = nullptr;

    Types m_types;
    TypesAlphabetic m_typesAlphabetic;

    const std::string m_name;

};
