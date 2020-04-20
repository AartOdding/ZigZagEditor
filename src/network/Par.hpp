#pragma once

#include <ZigZag/BaseParameter.hpp>


template<typename T>
class Par : public ZigZag::BaseParameter
{
public:

    Par(Object* parent = nullptr, std::string_view name = std::string())
        : BaseParameter(parent, name),
          m_value(),
          m_input()
    {

    }

    const T& value() const
    {
        return m_value;
    }
    
    virtual void process()
    {
        if (m_value != m_input)
        {
            m_value = m_input;
            supply(m_value);
        }
    }

    virtual void consume(const ZigZag::Variant& var)
    {
        if (var.canGet<T>())
        {
            m_input = var.get<T>();
        }
    }

private:

    T m_value;
    T m_input;

};
