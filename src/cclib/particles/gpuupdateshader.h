#ifndef __CCLIB_GPUUPDATESHADER_INCLUDED__
#define __CCLIB_GPUUPDATESHADER_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tr1/memory>
#include <GL/glew.h>

#include <cg/cg.h>
#include <cg/CgGL.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <Exception.h>

#include <gl/shader.h>

class GPUParticles;
typedef std::tr1::shared_ptr<GPUParticles> GPUParticlesPtr;

class GPUParticle;
typedef std::tr1::shared_ptr<GPUParticle> GPUParticlePtr;

class ShaderTexture;
typedef std::tr1::shared_ptr<ShaderTexture> ShaderTexturePtr;

class GPUForce;
typedef std::tr1::shared_ptr<GPUForce> GPUForcePtr;

class GPUConstraint;
typedef std::tr1::shared_ptr<GPUConstraint> GPUConstraintPtr;

class GPUImpulse;
typedef std::tr1::shared_ptr<GPUImpulse> GPUImpulsePtr;

class GPUConstraint;
typedef std::tr1::shared_ptr<GPUConstraint> GPUConstraintPtr;

namespace cclib {

class GPUUpdateShader : public Shader {

    public: 
        typedef std::tr1::shared_ptr<GPUUpdateShader> Ptr;
        
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

        GPUUpdateShader( GPUParticles theParticles, std::vector<GPUForcePtr> theForces , 
            std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
            const std::vector<std::string> & theShaderFile, int theWidth, int theHeight ); 
	
        Ptr create( GPUParticlesPtr theParticles, std::vector<GPUForcePtr> theForces, 
                std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
                std::vector<std::string> theShaderFile, int theWidth, int theHeight ); 
};

}; // namespace

#endif // includeguard
