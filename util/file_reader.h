#pragma once

#include "util/error.h"
#include <fstream>
#include <vector>
#include <sys/stat.h>


namespace util
{

class FileReader
{
public:
    FileReader( const std::string& filepath )
    {
        in_.open(filepath);
        if( !in_ )
        {
            THROW( InternalError, "File \"", filepath, "\" not found" );
        }
        size_ = GetFileSize(filepath);
    }

    ~FileReader()
    {
        in_.close();
    }

    std::string GetNextLine()
    {
        std::string line;
        std::getline( in_, line );
        return line;
    }

    std::vector<std::string> GetAllLines()
    {
        if ( in_.tellg() > 0 )
        {
            in_.seekg( 0 );
        }

        std::vector<std::string> lines;
        std::string line;
        while ( std::getline( in_, line ) )
        {
            lines.push_back( std::move(line) );
        }
        return lines;
    }

    operator bool()
    {
        return in_.tellg() < size_;
    }

protected:
    long GetFileSize( const std::string& filename )
    {
        struct stat stat_buf;
        if ( stat( filename.c_str(), &stat_buf ) < 0 )
        {
            THROW( util::InternalError, "stat error: ", strerror(errno) );
        }
        return stat_buf.st_size;
    }

private:
    std::ifstream in_;
    long size_ = 0;
};

}
