#pragma once

#include <json/json_traits.h>
#include <nlohmann/json.hpp>
#include <type_traits>

namespace json
{

class JsonConstRef;

class JsonRef
{
    friend JsonConstRef;
public:
    JsonRef( nlohmann::json& json ) : json_(json)
    {
    }

    nlohmann::json& operator*()
    {
        return json_;
    }

private:
    nlohmann::json& json_;
};

class JsonConstRef
{
public:
    JsonConstRef( const nlohmann::json& json ) : json_(json)
    {
    }

    JsonConstRef( JsonRef& json ) : json_(json.json_)
    {
    }

    const nlohmann::json& operator*() const
    {
        return json_;
    }

private:
    const nlohmann::json& json_;
};

template< typename T >
void ToJson( const T& inValue, JsonRef& jsonValue )
{
    auto& json = *jsonValue;
    if constexpr( IsPrimitive<T> || IsArray<T> )
    {
        json = inValue;
    }
    else if constexpr( IsEnum<T> )
    {
        json = std::underlying_type_t<T>(inValue);
    }
    else
    {
        // IsOptional<T> обрабатывается в вызывающей функции ToJson в json.h
        static_assert( IsPrimitive<T> || IsEnum<T> || IsArray<T> || IsObject<T> || IsOptional<T>,
                       "unknown type" );
        // Случай IsObject<T> должен обрабатывается внешне перегруженными FromJson<T>
        static_assert( !IsObject<T>, "there is no ToJson<T> overload" );
    }
}

template< typename T >
void FromJson( const JsonConstRef& jsonValue, T& outValue )
{
    auto& json = *jsonValue;
    if constexpr( IsPrimitive<T> )
    {
        outValue = json.get<T>();
    }
    else if constexpr( IsEnum<T> )
    {
        outValue = static_cast<T>( json.get<T>() );
    }
    else if constexpr( IsArray<T> )
    {
        auto& array = outValue;
        array.reserve( json.size() );
        for ( const auto& item : json )
        {
            typename T::value_type v;
            FromJson( item, v );
            array.push_back( std::move(v) );
        }
    }
    else
    {
        static_assert( IsPrimitive<T> || IsEnum<T> || IsArray<T> || IsObject<T> || IsOptional<T>,
                       "unknown type" );
        // Случай IsObject<T> должен обрабатывается внешне перегруженными FromJson<T>
        static_assert( !IsObject<T>, "there is no FromJson<T> overload" );
    }
}

}
