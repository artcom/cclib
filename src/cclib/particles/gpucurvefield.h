
#ifndef __CCLIB_GPUCURVEFIELD_INCLUDED__
#define __CCLIB_GPUCURVEFIELD_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUCurveField : public GPUForce {

    private: 
        float _myPrediction; // = 0;
        float _myOffset; // = 0;
        float _myScale; // = 1;
        float _myOutputScale; // = 1;
        float _myRadius; // = 1;
        float _mySpeed; // = 1;
        
        CGparameter _myPredictionParameter;
        CGparameter _myOffsetParameter;
        CGparameter _myScaleParameter;
        CGparameter _myOutputScaleParameter;
        CGparameter _myRadiusParameter;

        GPUCurveField(); 

    public:
        static GPUCurveFieldPtr create();

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void prediction(float thePrediction);
        void scale(float theScale);
        void outputScale(float theOutputScale);
        void radius(float theRadius);
        void speed(float theSpeed);
};	
};

#endif 
