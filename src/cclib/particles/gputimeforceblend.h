
#ifndef __CCLIB_GPUTIMEFORCEBLEND_INCLUDED__
#define __CCLIB_GPUTIMEFORCEBLEND_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUTimeForceBlend : public GPUForce {

    private:
        static int MAX_STATES;
        CGparameter _myStartTimeParameter;
        CGparameter _myEndTimeParameter;
    
        Texture2DPtr _myBlendInfos;
        std::vector<unsigned char> _myBlendInfoData;
        CGparameter _myBlendInfosParameter;
    
        Property_<float>::Ptr _myStartTime;
        Property_<float>::Ptr _myEndTime;
        Property_<float>::Ptr _myPower;

        CGparameter _myPowerParameter;
    
        GPUForcePtr _myForce1;
        GPUForcePtr _myForce2;
    
        GPUTimeForceBlend();

    public:
        GPUTimeForceBlend(float theStartTime,float theEndTime,GPUForcePtr theForce1,GPUForcePtr theForce2);
    
        static GPUTimeForceBlendPtr create();
        static GPUTimeForceBlendPtr create(float theStartTime,float theEndTime,GPUForcePtr theForce1,GPUForcePtr theForce2);
    
        void initialize(GPUForcePtr theForce1, GPUForcePtr theForce2); 

        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight);
        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader, std::string theIndex, int theWidth, int theHeight);
    
	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setSize(int theWidth, int theHeight);
        void setBlend(int theState, float theMinBlend, float theMaxBlend);
        void setBlend(float theMinBlend, float theMaxBlend);
};	
};

#endif 
