#include <object/ObjectType.hpp>



ObjectType::ObjectType(std::string_view name, Identifier identifier, ObjectTypeCategory category)
	: m_name(name)
	, m_identifier(identifier)
	, m_category(category)
{

}

const std::string& ObjectType::getName() const
{
	return m_name;
}

ObjectTypeCategory ObjectType::getCategory() const
{
	return m_category;
}


Identifier ObjectType::getIdentifier() const
{
	return m_identifier;
}
