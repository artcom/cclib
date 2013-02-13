
#ifndef __CCLIB_GPUATTRACTOR_INCLUDED__
#define __CCLIB_GPUATTRACTOR_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUAttractor : public GPUForce {

    private:
        float       _myRadius;
        Vector3fPtr _myPosition;
        
        CGparameter _myPositionParameter;
        CGparameter _myRadiusParameter;
	
        GPUAttractor(Vector3fPtr thePosition, float theStrength, float theRadius); 

    public:
        static GPUAttractorPtr create(Vector3fPtr thePosition, float theStrength, float theRadius);

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void position(Vector3fPtr thePosition);
        Vector3fPtr position();
        float getStrength();
        void radius(float theRadius);
        float radius();
    
};	
};

#endif 
