#ifndef __CCLIB_GPUCONSTRAINT_INCLUDED__
#define __CCLIB_GPUCONSTRAINT_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUConstraint {
	
    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShaderPtr _myVelocityShader;
        
    public: 
        GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, 
                float theMinimalVelocity); 
        virtual ~GPUConstraint() {};
        void setShader(GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight);
	
        CGparameter parameter(const std::string & theName);
        void resilience(float theResilience);
        void friction(float theFriction);	
        void minimalVelocity(float theMinimalVelocity); 
        void update(float theDeltaTime);
	
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
