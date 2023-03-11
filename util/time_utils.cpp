#include <util/time_utils.h>
#include <iomanip>
#include <stdexcept>
#include <string>

namespace util
{

TimePoint ToTimePoint( const std::string& dateTime, const std::string& format )
{
    std::stringstream ss;
    ss << dateTime;

    std::tm tm = {};
    ss >> std::get_time( &tm, format.c_str() );
    if ( ss.fail() )
    {
        throw std::runtime_error( dateTime );
    }
    return ToTimePoint( std::chrono::system_clock::from_time_t( std::mktime( &tm ) - timezone ) );
}

std::string ToString( TimePoint tp, const std::string& format )
{
    std::time_t t = std::chrono::system_clock::to_time_t( tp );
    std::tm tm = {};
    if ( !gmtime_r( &t, &tm ) )
    {
        throw std::runtime_error( std::to_string(t) );
    }

    std::stringstream ss;
    ss << std::put_time( &tm, format.c_str() );
    return ss.str();
}

}
