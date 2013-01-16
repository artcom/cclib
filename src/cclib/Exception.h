
#ifndef __CCLIB_EXCEPTION_INCLUDED__
#define __CCLIB_EXCEPTION_INCLUDED__

#include <string>
#include <stdexcept>

namespace cclib {

class Exception: public std::runtime_error
{
    public:
        Exception(std::string const& msg):
            std::runtime_error(msg)
        {}
};
    
} // namespace cclib 

#endif // __CCLIB_EXCEPTION_INCLUDED__
