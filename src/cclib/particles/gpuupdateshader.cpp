#include "gpuupdateshader.h"
#include <gl/shaderbuffer.h>
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
GPUUpdateShader::data(ShaderBufferPtr theDataBuffer) {
    texture(_myPositionTextureParameter, theDataBuffer->attachment(0)->id());
    texture(_myInfoTextureParameter,  theDataBuffer->attachment(1)->id());
    texture(_myVelocityTextureParameter, theDataBuffer->attachment(2)->id());
    texture(_myColorTextureParameter, theDataBuffer->attachment(3)->id());
}

void
GPUUpdateShader::deltaTime(float theDeltaTime) {
    parameter(_myDeltaTimeParameter, theDeltaTime);
}

GPUUpdateShader::GPUUpdateShader(const std::vector<std::string> & theShaderFile)
: CGShader(std::vector<std::string>(), theShaderFile),
    _myVelocityTextureParameter(),
    _myPositionTextureParameter(),
    _myInfoTextureParameter(),
    _myColorTextureParameter(),
    _myDeltaTimeParameter(),
    _myForcesParameter(),
    _myConstraintsParameter(),
    _myImpulsesParameter()
{
};

void GPUUpdateShader::setup(GPUUpdateShaderPtr theThis, GPUParticlesPtr theParticles, std::vector<GPUForcePtr> & theForces ,
        std::vector<GPUConstraintPtr> & theConstrains, std::vector<GPUImpulsePtr> & theImpulses, int theWidth, int theHeight)
{
    checkCGError("created velocity shader");

    _myForcesParameter = fragmentParameter("forces");
    cgSetArraySize(_myForcesParameter, theForces.size());

    int myIndex = 0;
    for(std::vector<GPUForcePtr>::size_type f=0; f<theForces.size(); f++) {
        GPUForcePtr myForce = theForces[f];
        myForce->setShader(theParticles, theThis, myIndex++, theWidth, theHeight);
    }

    _myConstraintsParameter = fragmentParameter("constraints");
    cgSetArraySize(_myConstraintsParameter, theConstrains.size());

    int myConstraintIndex = 0;
    for(std::vector<GPUConstraintPtr>::size_type c=0; c<theConstrains.size(); c++) {
        GPUConstraintPtr myConstraint = theConstrains[c];
        myConstraint->setShader(theThis, myConstraintIndex++, theWidth, theHeight);
    }

    _myImpulsesParameter = fragmentParameter("impulses");
    cgSetArraySize(_myImpulsesParameter, theImpulses.size());

    int myImpulseIndex = 0;
    for(std::vector<GPUImpulsePtr>::size_type i=0; i<theImpulses.size(); i++) {
        GPUImpulsePtr myImpulse = theImpulses[i];
        myImpulse->setShader(theThis, myImpulseIndex++, theWidth, theHeight);
    }

    _myPositionTextureParameter = fragmentParameter("positionTexture");
    _myInfoTextureParameter     = fragmentParameter("infoTexture");
    _myVelocityTextureParameter = fragmentParameter("velocityTexture");
    _myColorTextureParameter    = fragmentParameter("colorTexture");
    _myDeltaTimeParameter       = fragmentParameter("deltaTime");

    load();

    GPUNoise::attachFragmentNoise(theThis);
}

GPUUpdateShaderPtr 
GPUUpdateShader::create( GPUParticlesPtr theParticles, std::vector<GPUForcePtr> theForces,
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

    GPUUpdateShaderPtr obj = GPUUpdateShaderPtr(new GPUUpdateShader(theShaderFile));
    obj->setup(obj, theParticles, theForces, theConstrains, theImpulses, theWidth, theHeight);
    
    return obj;
}

