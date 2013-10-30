
#ifndef __CCLIB_GPUCURVEFIELD_INCLUDED__
#define __CCLIB_GPUCURVEFIELD_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUCurveField : public GPUForce {

    private: 
        Property_<float>::Ptr _myPrediction; 
        Property_<float>::Ptr _myOffset; 
        Property_<float>::Ptr _myScale; 
        Property_<float>::Ptr _myOutputScale;
        Property_<float>::Ptr _myRadius; 
        Property_<float>::Ptr _mySpeed;
        
        CGparameter _myPredictionParameter;
        CGparameter _myOffsetParameter;
        CGparameter _myScaleParameter;
        CGparameter _myOutputScaleParameter;
        CGparameter _myRadiusParameter;

        Texture2DPtr _myNoiseTexture;
        std::vector<unsigned char> _myNoiseData;
        CGparameter _myNoiseParameter;
        CGparameter _myNoiseTextureSizeParameter;
        CGparameter _myUseNoiseParameter;
    
        GPUCurveField();

    public:
        static GPUCurveFieldPtr create();
    
	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setPrediction(float thePrediction);
        void setScale(float theScale);
        void setOutputScale(float theOutputScale);
        void setRadius(float theRadius);
        void setSpeed(float theSpeed);
        void setOffset(float theOffset);

        float getPrediction();
        float getScale();
        float getOutputScale();
        float getRadius();
        float getSpeed();
        float getOffset();
};	
};

#endif 
