#ifndef __CCLIB_GPUIMPULSE_INCLUDED__
#define __CCLIB_GPUIMPULSE_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUImpulse {

    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShader _myVelocityShader;
	
    private:
        float _myStrength;
        CGparameter _myStrengthParameter;
        bool _myTrigger = false;
    
    public:	
        GPUImpulse(const std::string &theShaderTypeName, float theStrength);
        void setShader(GPUUpdateShader::Ptr theShader, int theIndex, int theWidth, int theHeight);
        virtual void setupParameter(int theWidth, int theHeight);

        CGparameter parameter(const std::string & theName);
        void strength(float theStrength);
        void trigger();
        bool isTriggering();
        void update(float theDeltaTime);
}

}; // namespace

#endif // includeguard

