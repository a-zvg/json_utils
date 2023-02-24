#pragma once

/// @file
/// @brief 

#include <util/error.h>
#include <json/detail/json_traits.h>
#include <json/detail/json_value.h>

namespace json
{

template< typename T >
void FromJson( const nlohmann::json& json, const char* const* it, std::optional< T >& t );
template< typename T, typename... Types >
void FromJson( const nlohmann::json& json, const char* const* it, std::optional< T >& t, Types&... args );

template< typename T >
EnableIfIsObject< T > FromJson( const nlohmann::json& json, const char* const* it, T& t )
{
    FromJson( json.at( *it ), t );
}

template< typename T, typename... Types >
EnableIfIsObject< T > FromJson( const nlohmann::json& json, const char* const* it, T& t, Types&... args )
{
    FromJson( json.at( *it ), t );
    FromJson( json, it + 1, args... );
}

template< typename T >
EnableIfIsValue< T > FromJson( const nlohmann::json& json, const char* const* it, T& t )
{
    try {
        t = static_cast< T >( JsonValue( json.at( *it ) ) );
    } catch ( const std::exception& ) {
        throw std::runtime_error( *it );
    }
}

template< typename T, typename... Types >
EnableIfIsValue< T > FromJson( const nlohmann::json& json, const char* const* it, T& t, Types&... args )
{
    FromJson( json, it, t );
    FromJson( json, it + 1, args... );
}

template< typename T >
void FromJson( const nlohmann::json& json, const char* const* it, std::optional< T >& t )
{
    if ( json.contains( *it ) )
    {
        T v;
        FromJson( json, it, v );
        t = std::move( v );
    }
}

template< typename T, typename... Types >
void FromJson( const nlohmann::json& json, const char* const* it, std::optional< T >& t, Types&... args )
{
    FromJson( json, it, t );
    FromJson( json, it + 1, args... );
}

} //namespace json
