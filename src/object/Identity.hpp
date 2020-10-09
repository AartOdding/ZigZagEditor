#pragma once

#include <atomic>
#include <cstdint>
#include <type_traits>

#include <object/Identifier.hpp>


/*
 * If Identifiable is constructed with an id value, the resulting Identifier
 * will be considered externally generated.
 */
template<typename Type>
class Identity
{
public:

    static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>);
    static_assert(std::is_base_of_v<Identity<Type>, Type>);

    Identity(Identity&&) = delete;
    Identity(const Identity&) = delete;

    Identity(T* object)
        : m_object(object)
        , m_external(true)
    {
        extern std::atomic_uint64_t nextIdentifiableValue;
        m_id = nextIdentifiableValue.fetch_add(1, std::memory_order_relaxed);
    }

    Identity(T* object, std::uint64_t id)
        : m_object(object)
        , m_id(id)
        , m_external(false)
    {
    }

    Identifier<T> getIdentifier();
    Identifier<const T> getIdentifier() const;

private:

    Type* m_object;
    std::uint64_t m_id;
    bool m_external;

};
