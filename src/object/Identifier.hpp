#pragma once

#include <cstdint>
#include <functional>


class Identifier
{
public:

	Identifier(std::uint64_t value);
	
	~Identifier() = default;
	Identifier(Identifier&&) = default;
	Identifier(const Identifier&) = default;

	bool operator==(Identifier rhs) const;
	bool operator!=(Identifier rhs) const;
	bool operator<(Identifier rhs) const;
	bool operator>(Identifier rhs) const;
	bool operator<=(Identifier rhs) const;
	bool operator>=(Identifier rhs) const;

	explicit operator std::uint64_t() const;

private:

	const std::uint64_t m_value;
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
