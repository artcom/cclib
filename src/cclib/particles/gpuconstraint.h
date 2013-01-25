#ifndef __CCLIB_GPUCONSTRAINT_INCLUDED__
#define __CCLIB_GPUCONSTRAINT_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <gl/gpuupdateshader.h>
#include <Exception.h>

namespace cclib {

class GPUConstraint {
	
    protected:
        String _myParameterIndex;
        String _myShaderTypeName;
        GPUUpdateShader::Ptr _myVelocityShader;
        
    public: 
        typedef std::tr1::shared_ptr<GPUConstraint> Ptr;
        
        GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, float theMinimalVelocity); 
        virtual ~GPUConstraint() {};
        void setShader(CCGPUUpdateShader::Ptr theShader, int theIndex, int theWidth, int theHeight);
	
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
