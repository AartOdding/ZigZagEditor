#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include <object/Identity.hpp>
#include <object/NodeCategory.hpp>

class NodeTemplateGroup;



class NodeTemplate : public Identity<NodeTemplate>
{
    NodeTemplate() = delete;
    NodeTemplate(NodeTemplate&&) = delete;
    NodeTemplate(const NodeTemplate&) = delete;

public:

    using Pointer = std::unique_ptr<NodeTemplate>;

    static Pointer create(std::string_view name, Identifier<NodeTemplate> identifier);
    ~NodeTemplate() = default;

    const std::string& getName() const;

    NodeCategory getCategory() const;
    void setCategory(NodeCategory category);

    NodeTemplateGroup* getNamespace();
    const NodeTemplateGroup* getNamespace() const;

private:

    friend class NodeTemplateGroup;

    NodeTemplate(std::string_view name, Identifier<NodeTemplate> identifier);

    const std::string m_name;
    NodeCategory m_category;
    NodeTemplateGroup* m_namespace;

};
