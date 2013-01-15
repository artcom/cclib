
#ifndef __CCLIB_EXCEPTION_INCLUDED__
#define __CCLIB_EXCEPTION_INCLUDED__

#include <string>

namespace cclib {
class Exception {
 
private:
    std::string _message;
 
public:
    Exception( std::string message ): _message(message) {};
    inline std::string getMessage() { 
        return _message; 
    };
    
    inline void log() { 
        std::cerr << _message;
    };
};
} // namespace cclib 

#endif // __CCLIB_EXCEPTION_INCLUDED__
