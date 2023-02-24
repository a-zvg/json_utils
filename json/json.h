#pragma once

/// @file
/// @brief 

#include <json/detail/json_convert_from.h>
#include <json/detail/json_convert_to.h>

template<typename... T>
constexpr std::array< typename std::common_type_t< T... >, sizeof...( T ) > make_array( T&&...t )
{
    return { std::forward< T >( t )... };
}

namespace json
{

template< std::size_t S, typename... Types >
void FromJson( const nlohmann::json& json, const std::array< const char*, S >& names, Types&... args )
{
    static_assert( names.size() == sizeof...( Types ) );
    FromJson( json, std::cbegin( names ), args... );
}

template< std::size_t S, typename... Types >
void ToJson( const std::array< const char*, S >& names, nlohmann::json& json, const Types&... args )
{
    static_assert( S == sizeof...( Types ) );
    ToJson( std::cbegin( names ), json, args... );
}

template< typename... Types >
void ToJson( nlohmann::json& json, const Types&... args )
{
    ToJson( json, args... );
}

//void Validate( const std::string& str, const char* name, std::size_t maxLength );

//template< typename T >
//struct JsonConverter
//{
    //static T FromJson( const nlohmann::json& json )
    //{
        //auto names = T::names_;
        //FromJson( json, names )
    //}
//};

}
