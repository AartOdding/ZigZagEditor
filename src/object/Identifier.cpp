#include <object/Identifier.hpp>


Identifier::Identifier(std::uint64_t value)
	: m_value(value)
{
}

bool Identifier::operator==(Identifier rhs) const
{
	return m_value == rhs.m_value;
}

bool Identifier::operator!=(Identifier rhs) const
{
	return m_value != rhs.m_value;
}

bool Identifier::operator<(Identifier rhs) const
{
	return m_value < rhs.m_value;
}

bool Identifier::operator>(Identifier rhs) const
{
	return m_value > rhs.m_value;
}

bool Identifier::operator<=(Identifier rhs) const
{
	return m_value <= rhs.m_value;
}

bool Identifier::operator>=(Identifier rhs) const
{
	return m_value >= rhs.m_value;
}

Identifier::operator std::uint64_t() const
{
	return m_value;
}
