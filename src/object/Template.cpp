#include <object/Template.hpp>



Template::Template(std::string_view name, Identifier<Template> identifier)
	: Identity<Template>(this, identifier)
	, m_name(name)
{
}

Template::Pointer Template::create(std::string_view name, Identifier<Template> identifier)
{
	return std::unique_ptr<Template>(new Template(name, identifier));
}

const std::string& Template::getName() const
{
	return m_name;
}

NodeCategory Template::getCategory() const
{
	return m_category;
}

void Template::setCategory(NodeCategory category)
{
	m_category = category;
}

TemplateGroup* Template::getNamespace()
{
	return m_namespace;
}

const TemplateGroup* Template::getNamespace() const
{
	return m_namespace;
}
