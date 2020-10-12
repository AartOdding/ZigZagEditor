#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <unordered_map>

#include <object/Identifier.hpp>


template<typename Type>
class Identity
{
    static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>);
    static_assert(std::is_base_of_v<Identity<Type>, Type>);

    Identifier<Type> m_identifier;

public:

    Identity(Identity&&) = delete;
    Identity(const Identity&) = delete;

    Identity(Type* identity);
    Identity(Type* identity, Identifier<Type> identifier);
    ~Identity();

    Identifier<Type> getIdentifier() const
    {
        return m_identifier;
    }

};


template<typename Type>
class IdentityMap
{
    static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>);
    static_assert(std::is_base_of_v<Identity<Type>, Type>);

    friend class Identity<Type>;

    static inline std::unordered_map<Identifier<Type>, Type*> m_map;

    static void add(Identifier<Type> identifier, Type* identity)
    {
        assert(identity);
        assert(identifier);
        assert(m_map.count(identifier) == 0);
        m_map[identifier] = identity;
    }

    static void remove(Identifier<Type> identifier)
    {
        assert(identifier);
        assert(m_map.count(identifier) == 1);
        m_map.erase(identifier);
    }

public:

    static const Type* get(Identifier<Type> identifier)
    {
        auto it = m_map.find(identifier);
        if (it != m_map.end()) return it->second;
        return nullptr;
    }

};


template<typename Type>
Identity<Type>::Identity(Type* identity)
{
    extern std::atomic_uint64_t nextIdentifiableValue;
    m_identifier = nextIdentifiableValue.fetch_add(1, std::memory_order_relaxed);
    IdentityMap<Type>::add(m_identifier, identity);
}

template<typename Type>
Identity<Type>::Identity(Type* identity, Identifier<Type> identifier)
    : m_identifier(identifier)
{
    IdentityMap<Type>::add(m_identifier, identity);
}

template<typename Type>
Identity<Type>::~Identity() 
{
    IdentityMap<Type>::remove(m_identifier);
}
