#pragma once

#include "util/logging.h"
#include <stdexcept>
#include <string.h>


#define THROW( ErrorType, ... ) \
    { LOG_WARN( "throw ", #ErrorType, " at ", __FILE__, ":", __LINE__ ); \
      throw ErrorType( std::make_pair( util::Join(__VA_ARGS__), util::Stacktrace( __FILE__, __LINE__ ) ) ); }

namespace util
{

std::string Stacktrace( const char* filename, int line );

class InternalError;
class BusinessError;

class Error : public std::runtime_error
{
    friend InternalError;
    friend BusinessError;

public:
    void Log() const
    {
        LOG_ERROR( what(), "\n", stacktrace_ );
    }

protected:
    Error( std::pair< std::string, std::string >&& info ) : std::runtime_error(info.first), stacktrace_( std::move(info.second) )
    {
        if ( stacktrace_.back() == '\n' )
        {
            stacktrace_.resize( stacktrace_.size()-1 );
        }
    }

private:
    std::string stacktrace_;
};

class InternalError : public Error
{
public:
    using Info = std::pair< std::string, std::string >;

    InternalError( Info&& info ) : Error( std::move(info) )
    {
    }
};

class BusinessError : public Error
{
public:
    using Info = std::pair< std::string, std::string >;

    BusinessError( Info&& info ) : Error( std::move(info) )
    {
    }
};

}

namespace detail
{
enum class BadRequest;
enum class Unauthenticated;
enum class Unauthorized;
enum class NotFound;
}

//using BusinessError = util::Exception; // Общий тип ошибки бизнес-логики
//using BadRequestError = util::Error< detail::BadRequest >; // Ошибка валидации входного запроса
//using AuthenticationError = util::Error< detail::Unauthenticated >; // Ошибка аутентификации
//using AuthorizationError = util::Error< detail::Unauthorized >; // Ошибка прав доступа
//using NotFoundError = util::Error< detail::NotFound >; // Ошибка поиска ресурса
