#ifndef __CCLIB_GPUIMPULSE_INCLUDED__
#define __CCLIB_GPUIMPULSE_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUImpulse {

    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShaderPtr _myVelocityShader;
	
    private:
        float _myStrength;
        CGparameter _myStrengthParameter;
        bool _myTrigger;
    
    public:	
        GPUImpulse(const std::string & theShaderTypeName, float theStrength);
        virtual void setShader(GPUUpdateShader * theShader, int theIndex, int theWidth, int theHeight);
        virtual void setupParameter(int theWidth, int theHeight) = 0;

        virtual CGparameter parameter(const std::string & theName);
        virtual void strength(float theStrength);
        virtual void trigger();
        virtual bool isTriggering();
        virtual void update(float theDeltaTime);
};

}; // namespace

#endif // includeguard

