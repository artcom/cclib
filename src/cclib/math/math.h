
#ifndef __CCLIB_MATH_INCLUDED__
#define __CCLIB_MATH_INCLUDED__

#include <cstdlib>

namespace cclib {

#define CC_DEG_TO_RAD M_PI / 180.0f
    
template<typename T>
T random(T theMin, T theMax) {
    T range = theMax - theMin;
    return range * ((T) rand()/RAND_MAX) + theMin; 

};

template<typename T>
T random(T theMax=1) {
    return cclib::random<T>(0, theMax);
};

template<typename T>
T radians(T theDegrees) {
    return theDegrees * (M_PI / 180.0);
}
    
}; // namespace



#endif
