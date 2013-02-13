
#ifndef __CCLIB_MATH_INCLUDED__
#define __CCLIB_MATH_INCLUDED__

namespace cclib {

template<typename T>
T random(T theMin, T theMax) {
    T range = theMax - theMin;
    return range * ((T) rand()/RAND_MAX) + theMin; 

};

template<typename T>
T random(T theMax=1) {
    return cclib::random<T>(0, theMax);
};

}; // namespace



#endif
