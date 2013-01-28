#ifndef __CCLIB_GPUUPDATESHADER_INCLUDED__
#define __CCLIB_GPUUPDATESHADER_INCLUDED__

#include <cclib.h>
#include <gl/shader.h>

namespace cclib {

class GPUUpdateShader : public Shader {

    public: 
        void positions(ShaderTexturePtr thePositionTexture);
        void deltaTime(float theDeltaTime);

    protected: 
	    CGparameter _myVelocityTextureParameter;
	    CGparameter _myPositionTextureParameter;
	    CGparameter _myInfoTextureParameter;
	    CGparameter _myDeltaTimeParameter;
	    CGparameter _myForcesParameter;
	    CGparameter _myConstraintsParameter;
	    CGparameter _myImpulsesParameter;

        GPUUpdateShader( GPUParticlesPtr theParticles, std::vector<GPUForcePtr> theForces , 
            std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
            const std::vector<std::string> & theShaderFile, int theWidth, int theHeight ); 
	
        GPUUpdateShaderPtr create( GPUParticlesPtr theParticles, std::vector<GPUForcePtr> theForces, 
                std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
                std::vector<std::string> theShaderFile, int theWidth, int theHeight ); 
};

}; // namespace

#endif // includeguard
