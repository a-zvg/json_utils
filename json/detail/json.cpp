/// @file
/// @brief 

#include <json_utils/json.h>

namespace json
{

void Validate( const std::string& str, const char* name, std::size_t maxLength )
{
    if ( str.size() > maxLength )
    {
        throw Error( ErrorCode::DataError, "некорректные", name, str );
    }
}

} //namespace json
