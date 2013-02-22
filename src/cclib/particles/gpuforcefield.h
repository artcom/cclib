
#ifndef __CCLIB_GPUFORCEFIELD_INCLUDED__
#define __CCLIB_GPUFORCEFIELD_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUForceField : public GPUForce {

    private:
        Property_<float>::Ptr _myNoiseScale;
        Property_<Vector3f>::Ptr _myNoiseOffset;
        
        CGparameter _myNoiseScaleParameter;
        CGparameter _myNoiseOffsetParameter;
        CGparameter _myNoiseLengthScalesParameter;
        CGparameter _myNoiseGainsParameter;
	
        GPUForceField(float theNoiseScale, float theStrength, Vector3f theNoiseOffset); 

    public:
        static GPUForceFieldPtr create(float theNoiseScale=1.0f, float theStrength=1.0f, Vector3f theNoiseOffset=Vector3f());

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setNoiseOffset(Vector3f theNoiseOffset);
        Vector3f getNoiseOffset();
        void setNoiseScale(float theNoiseScale);
        float getNoiseScale();
};	
};

#endif 
