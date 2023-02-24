#include "util/error.h"
#include "third_party/backward-cpp/backward.hpp"


namespace util
{

std::string Stacktrace( const char* filename, int line )
{
    namespace bt = backward;
    bt::StackTrace st;
    st.load_here();

    bt::TraceResolver tr;
    tr.load_stacktrace(st);

    std::ostringstream ss;
    for ( size_t i = 0, start = 0; i < st.size(); ++i )
    {
        bt::ResolvedTrace trace = tr.resolve( st[i] );
        using namespace std::string_literals;
        if ( !start && trace.object_function.find( "util::"s + __FUNCTION__ ) != std::string::npos )
        {
            start = i+1;
        }

        if ( start && i >= start )
        {
            ss << "  #" << i-start
                << " " << trace.object_filename
                << " " << trace.object_function;
            if ( i == start )
            {
                ss << " at " << filename << ":" << line;
            }
            ss << std::endl;
        }
        if ( trace.object_function == "main" )
        {
            break;
        }
    }
    return ss.str();
}

}

