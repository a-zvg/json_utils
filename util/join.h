#pragma once

#include <sstream>


namespace util
{

namespace detail
{

template < typename T >
inline void Join( std::stringstream& ss, const T& t )
{
    ss << t;
}

template < typename T, typename... Args >
inline void Join( std::stringstream& ss, const T& t, const Args&... args )
{
    ss << t;
    Join( ss, args... );
}

} //namespace detail

template < typename... Args >
inline std::string Join( const Args&... args )
{
    std::stringstream ss;
    detail::Join( ss, args... );
    return ss.str();
}

// Оптимизация для типов std::string и const char*, чтобы не использовался std::stringstream
template < typename T, typename = std::enable_if_t< std::is_convertible< T, std::string >::value > >
inline std::string Join( const T& str )
{
    return str;
}

inline std::string Join()
{
    return {};
}

}
