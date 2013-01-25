#include <gl/gpuupdateshader.h>

using namespace cclib;

void 
GPUUpdateShader::positions(ShaderTexture::Ptr thePositionTexture) {
    texture(_myPositionTextureParameter, thePositionTexture->id(0));
    texture(_myInfoTextureParameter, thePositionTexture->id(1));
    texture(_myVelocityTextureParameter, thePositionTexture->id(2));
}

void
GPUUpdateShader::deltaTime(float theDeltaTime) {
    parameter(_myDeltaTimeParameter, theDeltaTime);
}

GPUUpdateShader::GPUUpdateShader( GPUParticles theParticles, std::vector<GPUForce::Ptr> theForces , 
        std::vector<GPUConstraint::Ptr> theConstrains, std::vector<GPUImpulse::Ptr> theImpulses,
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
    for(std::vector<GPUForce::Ptr>::size_type f=0; f<theForces.size(); f++) {
        GPUForce::Ptr myForce = theForces[f];
        myForce->setShader(theParticles, Ptr(this), myIndex++, theWidth, theHeight);
    }

    _myConstraintsParameter = fragmentParameter("constraints");
    cgSetArraySize(_myConstraintsParameter, theConstrains.size());

    int myConstraintIndex = 0;
    for(std::vector<GPUConstraint::Ptr>::size_type c=0; c<theConstrains.size(); c++) {
        GPUConstraint::Ptr myConstraint = theConstrains[c];
        myConstraint->setShader(Ptr(this), myConstraintIndex++, theWidth, theHeight);
    }

    _myImpulsesParameter = fragmentParameter("impulses");
    cgSetArraySize(_myImpulsesParameter, theImpulses.size());

    int myImpulseIndex = 0;
    for(std::vector<GPUImpulse::Ptr>::size_type i=0; i<theImpulses.size(); i++) {
        GPUImpulse::Ptr myImpulse = theImpulses[i];
        myImpulse.setShader(this, myImpulseIndex++, theWidth, theHeight);
    }

    _myPositionTextureParameter = fragmentParameter("positionTexture");
    _myInfoTextureParameter     = fragmentParameter("infoTexture");
    _myVelocityTextureParameter = fragmentParameter("velocityTexture");
    _myDeltaTimeParameter       = fragmentParameter("deltaTime");

    load();

    GPUNoise::attachFragmentNoise(Ptr(this));
}

GPUUpdateShader::Ptr 
GPUUpdateShader::create( GPUParticles::PTr theParticles, std::vector<GPUForce::Ptr> theForces, 
        std::vector<CCGPUConstraint::Ptr> theConstrains, std::vector<CCGPUImpulse::Ptr> theImpulses,
        std::vector<std::string> theShaderFile, int theWidth, int theHeight) 
{
    if (theShaderFile.empty()) {
        theShaderFile.push_back(std::string(simple_fp));
        theShaderFile.push_back(std::string(forces_fp));
        theShaderFile.push_back(std::string(constraints_fp));
        theShaderFile.push_back(std::string(impulses_fp));
        theShaderFile.push_back(std::string(velocity_fp));
    }

    Ptr result = Ptr(new GPUParticles(theParticles, theForces, theConstrains, theImpulses, theShaderFile, theWidth, theHeight));        
    return result;
}

