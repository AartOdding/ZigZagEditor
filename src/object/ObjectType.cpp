#include <object/ObjectType.hpp>



ObjectType::ObjectType(std::string_view name, Identifier<ObjectType> identifier)
	: Identity<ObjectType>(this, identifier)
	, m_name(name)
{
}

ObjectType::Pointer ObjectType::create(std::string_view name, Identifier<ObjectType> identifier)
{
	return std::unique_ptr<ObjectType>(new ObjectType(name, identifier));
}

const std::string& ObjectType::getName() const
{
	return m_name;
}

ObjectTypeCategory ObjectType::getCategory() const
{
	return m_category;
}

void ObjectType::setCategory(ObjectTypeCategory category)
{
	m_category = category;
}

ObjectTypeNamespace* ObjectType::getNamespace()
{
	return m_namespace;
}

const ObjectTypeNamespace* ObjectType::getNamespace() const
{
	return m_namespace;
}
