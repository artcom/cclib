#ifndef __CCLIB_GPUUPDATESHADER_INCLUDED__
#define __CCLIB_GPUUPDATESHADER_INCLUDED__

#include <cclib.h>
#include <gl/shader.h>

namespace cclib {

class GPUUpdateShader : public Shader {

    public: 
        void data(ShaderBufferPtr theDataBuffer);
        void deltaTime(float theDeltaTime);
        
        static GPUUpdateShaderPtr create( GPUParticles * theParticles, std::vector<GPUForcePtr> theForces,
                std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
                std::vector<std::string> theShaderFile, int theWidth, int theHeight ); 

    protected: 
	    CGparameter _myVelocityTextureParameter;
	    CGparameter _myPositionTextureParameter;
	    CGparameter _myInfoTextureParameter;
        CGparameter _myColorTextureParameter;
        CGparameter _myDeltaTimeParameter;
    
	    CGparameter _myForcesParameter;
	    CGparameter _myConstraintsParameter;
	    CGparameter _myImpulsesParameter;

        GPUUpdateShader( GPUParticles * theParticles, std::vector<GPUForcePtr> & theForces ,
            std::vector<GPUConstraintPtr> & theConstrains, std::vector<GPUImpulsePtr> & theImpulses,
            const std::vector<std::string> & theShaderFile, int theWidth, int theHeight ); 
	
};

}; // namespace

#endif // includeguard
