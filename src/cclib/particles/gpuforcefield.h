
#ifndef __CCLIB_GPUFORCEFIELD_INCLUDED__
#define __CCLIB_GPUFORCEFIELD_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUForceField : public GPUForce {

    private:
        float _myNoiseScale;
        Vector3fPtr _myNoiseOffset;
        
        CGparameter _myNoiseScaleParameter;
        CGparameter _myNoiseOffsetParameter;
        CGparameter _myNoiseLengthScalesParameter;
        CGparameter _myNoiseGainsParameter;
	
        GPUForceField(float theNoiseScale, float theStrength, Vector3fPtr theNoiseOffset); 

    public:
        static GPUForceFieldPtr create(float theNoiseScale, float theStrength, Vector3fPtr theNoiseOffset);
        // GPUForcePtr getBasePtr();

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void noiseOffset(Vector3fPtr theNoiseOffset);
        Vector3fPtr noiseOffset();
        void noiseScale(float theNoiseScale);
        float noiseScale();
};	
};

#endif 
