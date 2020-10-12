#include <object/ObjectType.hpp>



ObjectType::ObjectType(Identifier<ObjectType> identifier)
	: Identity<ObjectType>(this, identifier)
{
}

ObjectType::Pointer ObjectType::create(Identifier<ObjectType> identifier)
{
	return std::unique_ptr<ObjectType>(new ObjectType(identifier));
}

const std::string& ObjectType::getName() const
{
	return m_name;
}

void ObjectType::setName(std::string_view name)
{
	m_name = name;
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
