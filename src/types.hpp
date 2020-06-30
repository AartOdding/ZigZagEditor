#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include <ZigZag/BaseDataInput.hpp>
#include <ZigZag/BaseDataSource.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/ObjectFactory.hpp>
#include <ZigZag/TParameter.hpp>




namespace TypeNames
{
    constexpr char baseDataInput  [] = "BaseDataInput";
    constexpr char baseDataSource [] = "BaseDataSource";
    constexpr char baseOperator   [] = "BaseOperator";

    constexpr char intParameter [] = "IntParameter";
    constexpr char int2Parameter[] = "Int2Parameter";
    constexpr char int3Parameter[] = "Int3Parameter";
    constexpr char int4Parameter[] = "Int4Parameter";

    constexpr char floatParameter [] = "FloatParameter";
    constexpr char float2Parameter[] = "Float2Parameter";
    constexpr char float3Parameter[] = "Float3Parameter";
    constexpr char float4Parameter[] = "Float4Parameter";
}


using IntParameter  = ZigZag::TParameter<int64_t,    TypeNames::intParameter>;
using Int2Parameter = ZigZag::TParameter<glm::ivec2, TypeNames::int2Parameter>;
using Int3Parameter = ZigZag::TParameter<glm::ivec3, TypeNames::int3Parameter>;
using Int4Parameter = ZigZag::TParameter<glm::ivec4, TypeNames::int4Parameter>;

using FloatParameter  = ZigZag::TParameter<double,    TypeNames::floatParameter>;
using Float2Parameter = ZigZag::TParameter<glm::vec2, TypeNames::float2Parameter>;
using Float3Parameter = ZigZag::TParameter<glm::vec3, TypeNames::float3Parameter>;
using Float4Parameter = ZigZag::TParameter<glm::vec4, TypeNames::float4Parameter>;



static void registerTypes()
{
    auto factory = ZigZag::ObjectFactory::instance();

    factory->registerType(TypeNames::baseDataInput,  [](){ return new ZigZag::BaseDataInput();  });
    factory->registerType(TypeNames::baseDataSource, [](){ return new ZigZag::BaseDataSource();  });
    factory->registerType(TypeNames::baseOperator,   [](){ return new ZigZag::BaseOperator();  });

    factory->registerType(TypeNames::intParameter,  [](){ return new IntParameter();  });
    factory->registerType(TypeNames::int2Parameter, [](){ return new Int2Parameter(); });
    factory->registerType(TypeNames::int3Parameter, [](){ return new Int3Parameter(); });
    factory->registerType(TypeNames::int4Parameter, [](){ return new Int4Parameter(); });
    
    factory->registerType(TypeNames::floatParameter,  [](){ return new FloatParameter();  });
    factory->registerType(TypeNames::float2Parameter, [](){ return new Float2Parameter(); });
    factory->registerType(TypeNames::float3Parameter, [](){ return new Float3Parameter(); });
    factory->registerType(TypeNames::float4Parameter, [](){ return new Float4Parameter(); });
}
