#pragma once

#include <json/json_base.h>
#include <stdexcept>

namespace json
{

template< typename T >
constexpr std::pair< T*, const char* > bind( T& v, const char* n )
{
    return { &v, n };
}

template< typename T >
void ToJson( JsonRef& jsonValue, const std::pair< T*, const char* > t )
{
    auto& json = *jsonValue;
    const auto& inValue = *t.first;
    auto key = t.second;

    if constexpr( IsOptional<T> )
    {
        if ( inValue.has_value() )
        {
            auto j = JsonRef( json[key] );
            ToJson( inValue.value(), j );
        }
    }
    else
    {
        auto j = JsonRef( json[key] );
        ToJson( inValue, j );
    }

}

template< typename... Types >
void ToJson( JsonRef& json, const Types... args )
{
    ( ToJson( json, args ), ... );
}

template< typename T >
void FromJson( const JsonConstRef& jsonValue, std::pair< T*, const char* > t )
{
    const auto& json = *jsonValue;
    auto& outValue = *t.first;
    auto key = t.second;

    if constexpr( IsOptional<T> )
    {
        if ( !json.contains(key) || json[key].is_null() )
        {
            outValue.reset();
        }
        else
        {
            outValue = typename T::value_type{};
            FromJson( JsonConstRef( json[key] ), outValue.value() );
        }
    }
    else
    {
        if ( !json.contains(key) || json[key].is_null() )
        {
            auto s = std::string(key) + ": not found";
            throw std::runtime_error(s);
        }
        FromJson( JsonConstRef( json[key] ), outValue );
    }
}

template< typename... Types >
void FromJson( const JsonConstRef& json, Types... args )
{
    ( FromJson( json, args ), ... );
}

}
