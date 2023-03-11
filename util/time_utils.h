#pragma once

#include <chrono>
#include <string>

namespace util
{

using TimeDuration = std::chrono::seconds;
using TimePoint = std::chrono::time_point< std::chrono::system_clock, TimeDuration >;

template< typename D >
inline TimePoint ToTimePoint( std::chrono::time_point< std::chrono::system_clock, D > t )
{
    return std::chrono::time_point_cast< TimeDuration >( t );
}

inline TimePoint TimeNow()
{
    return ToTimePoint( std::chrono::system_clock::now() );
};

TimePoint ToTimePoint( const std::string& dateTime, const std::string& format = "%d.%m.%Y %H:%M:%S" );

std::string ToString( TimePoint tp, const std::string& format = "%d.%m.%Y %H:%M:%S" );

}
