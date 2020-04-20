#pragma once

#include <type_traits>

#include <ZigZag/BaseDataInput.hpp>
#include <ZigZag/BaseDataSource.hpp>


template<typename DATA_TYPE>
class GenericInput : public ZigZag::BaseDataInput
{
public:

    static_assert(std::is_base_of<ZigZag::BaseDataSource, DATA_TYPE>::value);

    GenericInput(Object* parent, std::string_view name)
        : BaseDataInput(parent, name)
    { }

    bool canConnectWith(const ZigZag::BaseDataSource* dataSource) const
    {
        return dynamic_cast<DATA_TYPE*>(dataSource);
    }

    const DATA_TYPE* getData() const
    {
        return dynamic_cast<DATA_TYPE*>(getConnectedOutput());
    }

};
