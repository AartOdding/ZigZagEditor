#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <type_traits>
#include <string>


template<typename Type>
class Identifier
{
public:

    static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>);

	explicit Identifier(std::uint64_t id) : m_id(id) { }
	
	Identifier() = default;
	Identifier(const Identifier&) = default;
	Identifier& operator=(const Identifier&) = default;

	bool operator==(Identifier rhs) const { return m_id == rhs.m_id; }
	bool operator!=(Identifier rhs) const { return m_id != rhs.m_id; }
	bool operator<(Identifier rhs) const { return m_id < rhs.m_id; }
	bool operator>(Identifier rhs) const { return m_id > rhs.m_id; }
	bool operator<=(Identifier rhs) const { return m_id <= rhs.m_id; }
	bool operator>=(Identifier rhs) const { return m_id >= rhs.m_id; }

	explicit operator std::uint64_t() const { return m_id; }

	operator bool() const { return m_id != 0; }

	std::string toString() const { return "<" + std::to_string(m_id) + ">"; }

private:

	std::uint64_t m_id = 0;

};


namespace std
{
	template<typename Type> 
	struct hash<Identifier<Type>>
	{
		std::size_t operator()(const Identifier<Type>& id) const noexcept
		{
			return hash<std::uint64_t>()(static_cast<std::uint64_t>(id));
		}
	};
}

template<typename Type>
std::ostream& operator<<(std::ostream& os, Identifier<Type> const& id)
{
	return os << "<" << static_cast<std::uint64_t>(id) << ">";
}
