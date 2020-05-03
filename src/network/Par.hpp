#pragma once

#include <ZigZag/BaseParameter.hpp>


template<typename T>
class Par : public ZigZag::BaseParameter
{
public:

    Par(Object* parent = nullptr, std::string_view name = std::string())
        : BaseParameter(parent, name)
    {
        m_value.set(T());
    }

    const T& value() const
    {
        return m_value.get<T>();
    }

    template<typename T2>
    Par& operator=(T2&& value)
    {
        m_value.set<T2>(std::forward<T2>(value));
        return *this;
    }

};
