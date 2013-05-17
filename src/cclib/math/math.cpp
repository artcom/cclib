#include "math.h"
#include "simplexnoise.h"

float cclib::random3d(float x, float y, float z,float min, float max)
{
    return scaled_raw_noise_3d(min,max,x,y,z);
}

float cclib::random2d(float x, float y, float min, float max)
{
    return scaled_raw_noise_2d(min,max,x,y);
}