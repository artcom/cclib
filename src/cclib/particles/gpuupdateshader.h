#ifndef __CCLIB_GPUUPDATESHADER_INCLUDED__
#define __CCLIB_GPUUPDATESHADER_INCLUDED__

#include <cclib.h>

// shaders, defining simple_fp, forces_fp, constraints_fp, impulses_fp and velocity_fp
#include <stringified_shaders/simplex.fp.h>
#include <stringified_shaders/forces.fp.h>
#include <stringified_shaders/constraints.fp.h>
#include <stringified_shaders/impulses.fp.h>
#include <stringified_shaders/velocity.fp.h>

namespace cclib {

class GPUUpdateShader : public Shader {

    public: 
        typedef std::tr1::shared_ptr<GPUUpdateShaderShader> Ptr;
        
        void positions(ShaderTexture::Ptr thePositionTexture);
        void deltaTime(float theDeltaTime);

    protected: 
	    CGparameter _myVelocityTextureParameter;
	    CGparameter _myPositionTextureParameter;
	    CGparameter _myInfoTextureParameter;
	    CGparameter _myDeltaTimeParameter;
	    CGparameter _myForcesParameter;
	    CGparameter _myConstraintsParameter;
	    CGparameter _myImpulsesParameter;

        GPUUpdateShader( GPUParticles theParticles, std::vector<GPUForce::Ptr> theForces , 
            std::vector<GPUConstraint::Ptr> theConstrains, std::vector<GPUImpulse::Ptr> theImpulses,
            const std::vector<std::string> & theShaderFile, int theWidth, int theHeight ); 
	
        Ptr create( GPUParticles::Ptr theParticles, std::vector<GPUForce::Ptr> theForces, 
                std::vector<CCGPUConstraint::Ptr> theConstrains, std::vector<CCGPUImpulse::Ptr> theImpulses,
                std::vector<std::string> theShaderFile, int theWidth, int theHeight ); 
};

}; // namespace

#endif // includeguard
