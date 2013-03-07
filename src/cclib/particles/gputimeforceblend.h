
#ifndef __CCLIB_GPUATTRACTOR_INCLUDED__
#define __CCLIB_GPUATTRACTOR_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUAttractor : public GPUForce {

    private:
        Property_<float>::Ptr _myRadius;
        Property_<Vector3f>::Ptr _myPosition;
        
        CGparameter _myPositionParameter;
        CGparameter _myRadiusParameter;
	
        GPUAttractor(Vector3f thePosition, float theStrength, float theRadius); 

    public:
        static GPUAttractorPtr create(Vector3f thePosition = Vector3f(), 
                float theStrength = 1.0f, float theRadius = 1.0f);

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setPosition(Vector3f thePosition);
        Vector3f getPosition();
        float getStrength();
        void setRadius(float theRadius);
        float getRadius();
    
};	
};

#endif 
