#pragma once

#include "util/join.h"
#include <chrono>
#include <string>
#include <stdio.h>
#include <unistd.h>


namespace util
{

class ThisThread
{
public:
     ThisThread() = delete;
     ThisThread( const ThisThread& ) = delete;
     ThisThread& operator=( const ThisThread& ) = delete;

     static void SetName( const std::string& name )
     {
          GetSetName(name);
     }

     static const char* GetName()
     {
          return GetSetName();
     }

protected:
     static const char* GetSetName( const std::string& name = {} )
     {
          static thread_local std::string threadName_;

          if ( !name.empty() )
          {
               threadName_ = name;
          }
          return threadName_.c_str();
     }
};

}

#define TIMESTAMP std::chrono::system_clock::now().time_since_epoch().count() / 1'000'000'000.0
#define THREAD_ID gettid()

// printf гарантирует многопоточную корректность вывода сообщений
#define LOG_DEBUG( ... ) \
    { fprintf( stdout, "DEBUG %.3f %s[%i]: %s\n", TIMESTAMP, util::ThisThread::GetName(), THREAD_ID, util::Join(__VA_ARGS__).c_str() ); fflush(stdout); }

#define LOG_INFO( ... ) \
    { fprintf( stdout, "INFO %.3f %s[%i]: %s\n", TIMESTAMP, util::ThisThread::GetName(), THREAD_ID, util::Join(__VA_ARGS__).c_str() ); fflush(stdout); }

#define LOG_WARN( ... ) \
    fprintf( stderr, "WARN %.3f %s[%i]: %s\n", TIMESTAMP, util::ThisThread::GetName(), THREAD_ID, util::Join(__VA_ARGS__).c_str() )

#define LOG_ERROR( ... ) \
    fprintf( stderr, "ERROR %.3f %s[%i]: %s\n", TIMESTAMP, util::ThisThread::GetName(), THREAD_ID, util::Join(__VA_ARGS__).c_str() )
