#include "gpuupdateshader.h"
#include <gl/shadertexture.h>
#include <particles/gpuforce.h>
#include <particles/gpuconstraint.h>
#include <particles/gpuimpulse.h>
#include <particles/gpunoise.h>

// shaders, defining simple_fp, forces_fp, constraints_fp, impulses_fp and velocity_fp
#include <stringified_shaders/simplex.fp.h>
#include <stringified_shaders/forces.fp.h>
#include <stringified_shaders/constraints.fp.h>
#include <stringified_shaders/impulses.fp.h>
#include <stringified_shaders/velocity.fp.h>

using namespace cclib;

void 
GPUUpdateShader::positions(ShaderTexturePtr thePositionTexture) {
    texture(_myPositionTextureParameter, thePositionTexture->id(0));
    texture(_myInfoTextureParameter, thePositionTexture->id(1));
    texture(_myVelocityTextureParameter, thePositionTexture->id(2));
}

void
GPUUpdateShader::deltaTime(float theDeltaTime) {
    parameter(_myDeltaTimeParameter, theDeltaTime);
}

GPUUpdateShader::GPUUpdateShader( GPUParticles * theParticles, std::vector<GPUForcePtr> & theForces ,
        std::vector<GPUConstraintPtr> & theConstrains, std::vector<GPUImpulsePtr> & theImpulses,
        const std::vector<std::string> & theShaderFile, int theWidth, int theHeight) 
: Shader(std::vector<std::string>(), theShaderFile),
    _myVelocityTextureParameter(),
    _myPositionTextureParameter(),
    _myInfoTextureParameter(),
    _myDeltaTimeParameter(),
    _myForcesParameter(),
    _myConstraintsParameter(),
    _myImpulsesParameter()
{
    checkError("created velocity shader");

    _myForcesParameter = fragmentParameter("forces");
    cgSetArraySize(_myForcesParameter, theForces.size());

    int myIndex = 0;
    for(std::vector<GPUForcePtr>::size_type f=0; f<theForces.size(); f++) {
        GPUForcePtr myForce = theForces[f];
        myForce->setShader(theParticles,  this, myIndex++, theWidth, theHeight);
    }

    _myConstraintsParameter = fragmentParameter("constraints");
    cgSetArraySize(_myConstraintsParameter, theConstrains.size());

    int myConstraintIndex = 0;
    for(std::vector<GPUConstraintPtr>::size_type c=0; c<theConstrains.size(); c++) {
        GPUConstraintPtr myConstraint = theConstrains[c];
        myConstraint->setShader(this, myConstraintIndex++, theWidth, theHeight);
    }

    _myImpulsesParameter = fragmentParameter("impulses");
    cgSetArraySize(_myImpulsesParameter, theImpulses.size());

    int myImpulseIndex = 0;
    for(std::vector<GPUImpulsePtr>::size_type i=0; i<theImpulses.size(); i++) {
        GPUImpulsePtr myImpulse = theImpulses[i];
        myImpulse->setShader(this, myImpulseIndex++, theWidth, theHeight);
    }

    _myPositionTextureParameter = fragmentParameter("positionTexture");
    _myInfoTextureParameter     = fragmentParameter("infoTexture");
    _myVelocityTextureParameter = fragmentParameter("velocityTexture");
    _myDeltaTimeParameter       = fragmentParameter("deltaTime");

    load();

    GPUNoise::attachFragmentNoise(this);
}

GPUUpdateShaderPtr 
GPUUpdateShader::create( GPUParticles * theParticles, std::vector<GPUForcePtr> theForces,
        std::vector<GPUConstraintPtr> theConstrains, std::vector<GPUImpulsePtr> theImpulses,
        std::vector<std::string> theShaderFile, int theWidth, int theHeight) 
{
    if (theShaderFile.empty()) {
        theShaderFile.push_back(std::string(simplex_fp));
        theShaderFile.push_back(std::string(forces_fp));
        theShaderFile.push_back(std::string(constraints_fp));
        theShaderFile.push_back(std::string(impulses_fp));
        theShaderFile.push_back(std::string(velocity_fp));
    }

    GPUUpdateShaderPtr result = GPUUpdateShaderPtr(new GPUUpdateShader(theParticles, theForces, theConstrains, theImpulses, theShaderFile, theWidth, theHeight));        
    return result;
}

