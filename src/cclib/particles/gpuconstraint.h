#ifndef __CCLIB_GPUCONSTRAINT_INCLUDED__
#define __CCLIB_GPUCONSTRAINT_INCLUDED__

#include <cclib.h>
#include <base/component.h>

namespace cclib {

class GPUConstraint : public Component {
	
    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShaderPtr _myVelocityShader;
        
    public: 
        GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, 
                float theMinimalVelocity); 
        virtual ~GPUConstraint() {};
        virtual void setShader(GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight);
	
        virtual CGparameter parameter(const std::string & theName);
        virtual void resilience(float theResilience);
        virtual void friction(float theFriction);
        virtual void minimalVelocity(float theMinimalVelocity);
        virtual void update(float theDeltaTime);
	
        virtual void setupParameter(int theWidth, int theHeight) = 0;
	
    private:
        float _myResilience;
        float _myFriction;
        float _myMinimalVelocity;
        
        CGparameter _myResilienceParameter;
        CGparameter _myFrictionarameter;
        CGparameter _myMinimalVelocityParameter;
};

}; // namespace

#endif // includeguard
