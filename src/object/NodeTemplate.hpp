#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include <object/Identity.hpp>
#include <object/NodeCategory.hpp>

class TemplateGroup;



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

    TemplateGroup* getNamespace();
    const TemplateGroup* getNamespace() const;

private:

    friend class TemplateGroup;

    NodeTemplate(std::string_view name, Identifier<NodeTemplate> identifier);

    const std::string m_name;
    NodeCategory m_category;
    TemplateGroup* m_namespace;

};
