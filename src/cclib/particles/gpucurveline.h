
#ifndef __CCLIB_GPUCURVELINE_INCLUDED__
#define __CCLIB_GPUCURVELINE_INCLUDED__

#include <vector>

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUCurveLine: public GPUForce {

    private: 
        Property_<float>::Ptr _myPrediction; 
        Property_<float>::Ptr _myRadius; 
        Property_<float>::Ptr _myMinX;
        Property_<float>::Ptr _myMaxX;
        Property_<float>::Ptr _myOutputScale;
        Property_<std::vector<cclib::Vector3f> >::Ptr _myCurvePoints;
        
        CGparameter _myPredictionParameter;
        CGparameter _myRadiusParameter;
        CGparameter _myOutputScaleParameter;
        CGparameter _myMinXParameter;
        CGparameter _myRangeXParameter;
        CGparameter _myRadiusParmeter;
        CGparameter _myCurveTextureParameter;
    
        cclib::ShaderBufferPtr _myCurveData;
        cclib::CGShaderPtr _myInitValueShader;

        GPUCurveLine();

    public:
        static GPUCurveLinePtr create();
    
	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
};	
};

#endif 
