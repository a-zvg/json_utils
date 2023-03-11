#pragma once

#include <util/time_utils.h>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace json
{

template<typename> struct IsArrayT : public std::false_type {};
template< typename T, typename A >
struct IsArrayT< std::vector< T, A > > : public std::true_type {};

template<typename> struct IsOptionalT : public std::false_type {};
template< typename T >
struct IsOptionalT< std::optional<T> > : public std::true_type {};

template< typename T >
inline constexpr bool IsPrimitive = std::is_arithmetic_v<T> ||
                                    std::is_same_v< std::remove_const_t<T>, std::string >;
template< typename T >
inline constexpr bool IsEnum = std::is_enum_v<T>;

template< typename T >
inline constexpr bool IsArray = IsArrayT< std::remove_const_t<T> >::value;

template< typename T >
inline constexpr bool IsObject = std::is_aggregate_v< std::remove_const_t<T> >;

template< typename T >
inline constexpr bool IsOptional = IsOptionalT< std::remove_const_t<T> >::value;

}
