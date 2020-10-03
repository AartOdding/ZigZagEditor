#include <object/ObjectType.hpp>



ObjectType::ObjectType(std::string_view name, Identifier uniqueID, ObjectTypeCategory category)
	: m_name(name)
	, m_uniqueID(uniqueID)
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


Identifier ObjectType::getUniqueID() const
{
	return m_uniqueID;
}
