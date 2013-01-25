#ifndef __CCLIB_GPUCONSTRAINT_INCLUDED__
#define __CCLIB_GPUCONSTRAINT_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUConstraint {
	
    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShader::Ptr _myVelocityShader;
        
    public: 
        typedef std::tr1::shared_ptr<GPUConstraint> Ptr;
        
        GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, float theMinimalVelocity); 
        virtual ~GPUConstraint() {};
        void setShader(GPUUpdateShader::Ptr theShader, int theIndex, int theWidth, int theHeight);
	
        CGparameter parameter(const std::string & theName);
        void resilience(float theResilience);
        void friction(final float theFriction);	
        void minimalVelocity(final float theMinimalVelocity); 
        void update(final float theDeltaTime);
	
        virtual void setupParameter(int theWidth, int theHeight);
	
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
