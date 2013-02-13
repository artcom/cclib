
#ifndef __CCLIB_GPUGRAVITY_INCLUDED__
#define __CCLIB_GPUGRAVITY_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUGravity : public GPUForce {

    private:
        CGparameter _myGravityParameter;
        Vector3fPtr _myGravity;

        GPUGravity(Vector3fPtr theGravity); 

    public:
        static GPUGravityPtr create(Vector3fPtr theGravity);

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        Vector3fPtr direction();
};	
};

#endif 
