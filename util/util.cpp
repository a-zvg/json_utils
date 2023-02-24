#include "util/util.h"
#include "util/error.h"
#include "util/strutil.h"
#include <fcntl.h>
#include <unistd.h>


namespace util
{

std::string ReadSome( int fd )
{
    char buf[1024] = { 0, };
    std::string out;
    int n = 0;
    while ( ( n = read( fd, buf, sizeof(buf) ) ) > 0 )
    {
        out += std::string( buf, n );
    }
    if ( n < 0 )
    {
        if ( errno != EAGAIN ) // Вдруг дескриптор неблокирующий
        {
            THROW( InternalError, "read error: ", strerror(errno) );
        }
    }
    return out;
}

ProgInfo ParseCmd( const std::string& cmd )
{
     ProgInfo result;
     std::vector<std::string> res = strutil::split( cmd, " " );
     for ( auto& t : res )
     {
          if ( result.count( "prog" ) == 0 )
          {
               if ( t.find( "=" ) < t.find( "/" ) )
               {
                    result["env"].push_back(t);
               }
               else
               {
                    result["prog"].push_back(t);
               }
          }
          else
          {
               result["args"].push_back(t);
          }
     }

     auto GetValues = []( const std::vector< std::string >& values ) -> std::string
     {
          std::stringstream ss;
          for ( const auto& v : values )
          {
               ss << "[" << v << "] ";
          }
          return ss.str();
     };

     LOG_DEBUG( "env: ", GetValues( result["env"] ) );
     LOG_DEBUG( "prog: ", GetValues( result["prog"] ) );
     LOG_DEBUG( "args: ", GetValues( result["args"] ) );

     return result;
}

ChildInfo ExecProg( const ProgInfo& info )
{
    // Подготовка к запуску дочернего процесса
    int out[2] = { 0, 0 };
    if( pipe2( out, O_CLOEXEC | O_NONBLOCK ) < 0 )
    {
        THROW( util::InternalError, "pipe2 error: ", strerror(errno) );
    }
    int err[2] = { 0, 0 };
    if( pipe2( err, O_CLOEXEC | O_NONBLOCK ) < 0 )
    {
        THROW( util::InternalError, "pipe2 error: ", strerror(errno) );
    }

    pid_t pid = fork();
    if ( pid < 0 )
    {
        THROW( util::InternalError, "fork error: ", strerror(errno) );
    }
    else if ( pid == 0 )
    {
        // Не забывать закрывать лишние дескрипторы, иначе они наследуются при exec
        // Обработчики сигналов при exec сбрасываются автоматически

        if ( dup2( out[1], STDOUT_FILENO ) < 0 )
        {
            THROW( util::InternalError, "dup2 error: ", strerror(errno) );
        }
        if ( dup2( err[1], STDERR_FILENO ) < 0 )
        {
            THROW( util::InternalError, "dup2 error: ", strerror(errno) );
        }

        const std::string& prog = info.at("prog")[0];
        std::vector<char*> args( info.at("args").size() + 1 );
        args[0] = const_cast<char*>( prog.c_str() );
        std::transform( info.at("args").begin(), info.at("args").end(), args.begin()+1, [](auto& str) {
              return const_cast<char*>( str.c_str() );
        } );
        args.push_back(nullptr);

        std::vector<char*> env( info.at("env").size() );
        std::transform( info.at("env").begin(), info.at("env").end(), env.begin(), [](auto& str) {
              return const_cast<char*>( str.c_str() );
        } );
        env.push_back(nullptr);

        execve( prog.c_str(), args.data(), env.data() );
        fprintf( stderr, "execve error: %s", strerror(errno) );
        exit(1);
    }

    close( out[1] );
    close( err[1] );

    auto tokens = strutil::split( info.at("prog")[0], "/" );
    std::string name = tokens.back();
    LOG_INFO( name, "[", pid, "] try to start..." );

    return ChildInfo{ .pid_ = pid, .name_ = name, .outFd_ = out[0], .errFd_ = err[0] };
}

}
