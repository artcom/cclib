
#ifndef __CCLIB_MATH_INCLUDED__
#define __CCLIB_MATH_INCLUDED__

#include <cstdlib>
#include <math.h>

namespace cclib {

//#define PI 3.14159265358979323846
#define DEG_TO_RAD PI / 180.0f
    
template<typename T>
T random(T theMin, T theMax) {
    T range = theMax - theMin;
    return range * ((T) rand()/RAND_MAX) + theMin; 

};

template<typename T>
T random(T theMax=1) {
    return cclib::random<T>(0, theMax);
};

// simplex noise random, output range [-1,1]
float random3d(float x, float y, float z,float min=-1, float max=1);
float random2d(float x, float y, float min=-1, float max=1);
    
template<typename T>
T radians(T theDegrees) {
    return theDegrees * (M_PI / 180.0);
}
 
    

}; // namespace



#endif
