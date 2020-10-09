#pragma once

#include <cstdint>
#include <functional>


template<typename T>
class Identifier
{
public:

	Identifier()
		: m_id(0)
		, m_object(nullptr)
		, m_external(false)
	{
	}

	Identifier(std::uint64_t id, T* object, bool external)
		: m_id(id)
		, m_object(object)
		, m_external(external)
	{	
	}
	
	~Identifier() = default;
	Identifier(const Identifier&) = default;
	Identifier& operator=(const Identifier&) = default;

	bool operator==(Identifier rhs) const;
	bool operator!=(Identifier rhs) const;
	bool operator<(Identifier rhs) const;
	bool operator>(Identifier rhs) const;
	bool operator<=(Identifier rhs) const;
	bool operator>=(Identifier rhs) const;

	explicit operator std::uint64_t() const;

	operator bool() const;

private:

	const std::uint64_t m_id;
    T* const m_object;
	const bool m_external;

};


namespace std
{
	template <> struct hash<Identifier>
	{
		std::size_t operator()(const Identifier& id) const noexcept
		{
			return hash<std::uint64_t>()(static_cast<std::uint64_t>(id));
		}
	};
}
