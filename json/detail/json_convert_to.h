#pragma once

/// @file
/// @brief 

#include <util/error.h>
#include <json/detail/json_traits.h>
#include <json/detail/json_value.h>

namespace json
{

template< typename T >
void ToJson( const char* const* it, nlohmann::json& json, const std::optional< T >& t );
template< typename T, typename... Types >
void ToJson( const char* const* it, nlohmann::json& json, const std::optional< T >& t, const Types&... args );

template< typename T >
EnableIfIsObject< T > ToJson( const char* const* it, nlohmann::json& json, const T& t )
{
    ToJson( t, json[ *it ] );
}

template< typename T, typename... Types >
EnableIfIsObject< T > ToJson( const char* const* it, nlohmann::json& json, const T& t, const Types&... args )
{
    ToJson( t, json[ *it ] );
    ToJson( it + 1, json, args... );
}

template< typename T >
EnableIfIsValue< T > ToJson( const char* const* it, nlohmann::json& json, const T& t )
{
    try {
        auto value = JsonValue( t ).value();
        json[ *it ] = std::move( value );
    } catch ( const std::exception& ) {
        throw std::runtime_error( *it );
    }
}

template< typename T, typename... Types >
EnableIfIsValue< T > ToJson( const char* const* it, nlohmann::json& json, const T& t, const Types&... args )
{
    ToJson( it, json, t );
    ToJson( it + 1, json, args... );
}

template< typename T >
void ToJson( const char* const* it, nlohmann::json& json, const std::optional< T >& t )
{
    if ( t.has_value() )
    {
        ToJson( it, json, *t );
    }
}

template< typename T, typename... Types >
void ToJson( const char* const* it, nlohmann::json& json, const std::optional< T >& t, const Types&... args )
{
    ToJson( it, json, t );
    ToJson( it + 1, json, args... );
}

} //namespace json
