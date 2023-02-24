#pragma once

#include <util/time_utils.h>
#include <nlohmann/json.hpp>
#include <vector>

namespace json
{

class JsonValue
{
public:
    JsonValue( const nlohmann::json& value ) : iValue_( value )
    {
    }

    JsonValue( const TimePoint& value ) : iValue_( oValue_ )
    {
        oValue_ = PutTime( value );
    }

    template< typename T >
    JsonValue( const T& value ) : iValue_( oValue_ )
    {
        oValue_ = value;
    }

    nlohmann::json value() const
    {
        return oValue_;
    }

    explicit operator std::string() const
    {
        if ( !iValue_.is_string() )
        {
            throw std::runtime_error( "" );
        }
        return iValue_.get< std::string >();
    }

    explicit operator std::int64_t() const
    {
        if ( !iValue_.is_number_integer() )
        {
            throw std::runtime_error( "" );
        }
        return iValue_.get< std::int64_t >();
    }

    explicit operator TimePoint() const
    {
        std::string value = static_cast< std::string >( *this );
        return GetTime( value );
    }

    template< typename T >
    explicit operator std::vector< T >() const
    {
        std::vector< T > array;
        array.reserve( iValue_.size() );
        for ( auto it = iValue_.begin(); it != iValue_.end(); ++it )
        {
            array.push_back( static_cast< T >( JsonValue( *it ) ) );
        }
        return array;
    }

private:
    nlohmann::json oValue_;
    const nlohmann::json& iValue_;
};

} //namespace json
