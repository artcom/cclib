
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
        TextureDataPtr _myBlendInfoData;
        CGparameter _myBlendInfosParameter;
    
        float _myStartTime;
        float _myEndTime;
    
        CGparameter _myPowerParameter;
        float _myPower;
    
        GPUForcePtr _myForce1;
        GPUForcePtr _myForce2;
    
        GPUTimeForceBlend(float theStartTime, float theEndTime, GPUForcePtr theForce1, GPUForcePtr theForce2);

    public:
        static GPUTimeForceBlendPtr create(float theStartTime, float theEndTime, GPUForcePtr theForce1, GPUForcePtr theForce2);

        void setShader(GPUParticles * theParticles, GPUUpdateShader * theShader, int theIndex, int theWidth, int theHeight);
        void setShader(GPUParticles * theParticles, GPUUpdateShader * theShader, std::string theIndex, int theWidth, int theHeight);
    
	    void setupParameter(int theWidth, int theHeight);
        void setSize(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void startTime(float theStartTime);
        void endTime(float theEndTime);
        void blend(int theState, float theMinBlend, float theMaxBlend);
        void blend(float theMinBlend, float theMaxBlend);
        void power(float thePower);
};	
};

#endif 
