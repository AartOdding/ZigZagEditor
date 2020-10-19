#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include <object/Identity.hpp>
#include <object/NodeCategory.hpp>

class TemplateGroup;



class Template : public Identity<Template>
{
    Template() = delete;
    Template(Template&&) = delete;
    Template(const Template&) = delete;

public:

    using Pointer = std::unique_ptr<Template>;

    static Pointer create(std::string_view name, Identifier<Template> identifier);
    ~Template() = default;

    const std::string& getName() const;

    NodeCategory getCategory() const;
    void setCategory(NodeCategory category);

    TemplateGroup* getNamespace();
    const TemplateGroup* getNamespace() const;

private:

    friend class TemplateGroup;

    Template(std::string_view name, Identifier<Template> identifier);

    const std::string m_name;
    NodeCategory m_category;
    TemplateGroup* m_namespace;

};
