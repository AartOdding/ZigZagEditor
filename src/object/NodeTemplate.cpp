#include <object/NodeTemplate.hpp>



NodeTemplate::NodeTemplate(std::string_view name, Identifier<NodeTemplate> identifier)
	: Identity<NodeTemplate>(this, identifier)
	, m_name(name)
{
}

NodeTemplate::Pointer NodeTemplate::create(std::string_view name, Identifier<NodeTemplate> identifier)
{
	return std::unique_ptr<NodeTemplate>(new NodeTemplate(name, identifier));
}

const std::string& NodeTemplate::getName() const
{
	return m_name;
}

NodeCategory NodeTemplate::getCategory() const
{
	return m_category;
}

void NodeTemplate::setCategory(NodeCategory category)
{
	m_category = category;
}

NodeTemplateGroup* NodeTemplate::getNamespace()
{
	return m_namespace;
}

const NodeTemplateGroup* NodeTemplate::getNamespace() const
{
	return m_namespace;
}
