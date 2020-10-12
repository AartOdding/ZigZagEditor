#pragma once

#include <cstdint>
#include <functional>
#include <type_traits>


template<typename T>
class Identifier
{
public:

    static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>);
    static_assert(std::is_base_of_v<Identity<Type>, Type>);

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

private:

	const std::uint64_t m_id = 0;

};


namespace std
{
	template<typename T> 
	struct hash<Identifier<T>>
	{
		std::size_t operator()(const Identifier<T>& id) const noexcept
		{
			return hash<std::uint64_t>()(static_cast<std::uint64_t>(id));
		}
	};
}
