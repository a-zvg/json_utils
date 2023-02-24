#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace util
{

struct ChildInfo
{
    pid_t pid_ = 0;
    std::string name_;
    int outFd_ = 0;
    int errFd_ = 0;
};

using ProgInfo = std::unordered_map< std::string, std::vector<std::string> >;

std::string ReadSome( int fd );

ProgInfo ParseCmd( const std::string& cmd );

ChildInfo ExecProg( const ProgInfo& info );

}
