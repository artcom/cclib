
#ifndef __CCLIB_GPUGRAVITY_INCLUDED__
#define __CCLIB_GPUGRAVITY_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUGravity : public GPUForce {

    private:
        CGparameter _myGravityParameter;
        Property_<Vector3f>::Ptr _myGravity;

        GPUGravity(Vector3f theGravity); 

    public:
        static GPUGravityPtr create(Vector3f theGravity=Vector3f());

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        Vector3f getDirection();
        void setDirection(Vector3f theGravity);
};	
};

#endif 
