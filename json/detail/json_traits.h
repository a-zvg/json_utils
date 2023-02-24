#pragma once

/// @file
/// @brief 

#include <util/time_utils.h>

#include <optional>
#include <type_traits>
#include <string>
#include <vector>

namespace json
{

template< typename > struct IsVector : public std::false_type {};
template< typename T, typename A >
struct IsVector< std::vector< T, A > > : public std::true_type {};

template< typename > struct IsOptional : public std::false_type {};
template< typename T >
struct IsOptional< std::optional< T > > : public std::true_type {};

template< typename T >
using EnableIfIsValue = std::enable_if_t< std::is_arithmetic_v< T > || std::is_same_v< T, std::string > ||
                        std::is_same_v< T, TimePoint > || IsVector< T >::value, void >;

template< typename T >
using EnableIfIsObject = std::enable_if_t< std::is_class_v< T > && !std::is_same_v< T, std::string > &&
                         !std::is_same_v< T, TimePoint > && !IsVector< T >::value && !IsOptional< T >::value, void >;

}
