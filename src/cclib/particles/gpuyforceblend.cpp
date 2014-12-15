
#include "gpuyforceblend.h"
#include <gl/texture2d.h>

using namespace cclib; 

GPUYForceBlend::GPUYForceBlend() : 
    GPUForce("YForceBlend"),
    _myY1(Property_<float>::create("y1", 20.0f)),
    _myY2(Property_<float>::create("y2", -20.0f)),
    _myBlendWidth(Property_<float>::create("blendWidth", 5.0f))
{
    registerProperty(_myY1);
    registerProperty(_myY2);
    registerProperty(_myBlendWidth);
}

void
GPUYForceBlend::initialize(GPUForcePtr theForce1, GPUForcePtr theForce2) 
{
    _myForce1 = theForce1;
    _myForce2 = theForce2;
}

GPUYForceBlendPtr
GPUYForceBlend::create() {
    return GPUYForceBlendPtr(new GPUYForceBlend());
}

void
GPUYForceBlend::setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
        int theIndex, int theWidth, int theHeight) 
{
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    
    std::stringstream ss;
    ss << "forces[" << theIndex << "]";
    
    _myParameterIndex = ss.str();

    cgConnectParameter( _myVelocityShader->createFragmentParameter(_myShaderTypeName), 
            _myVelocityShader->fragmentParameter(_myParameterIndex));

    setupParameter(theWidth, theHeight);
    _myVelocityShader->checkCGError("Problem creating force.");
    
    std::stringstream ss1;
    ss1 << _myParameterIndex << ".force1";

    std::stringstream ss2;
    ss2 << _myParameterIndex << ".force2";
    
    _myForce1->setShader(theParticles, theShader, ss1.str(), theWidth, theHeight);
    _myForce2->setShader(theParticles, theShader, ss2.str(), theWidth, theHeight);
}

void
GPUYForceBlend::setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
        std::string theIndex, int theWidth, int theHeight) 
{
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    _myParameterIndex = theIndex;
    _myVelocityShader->checkCGError("Problem creating force.");
    
    cgConnectParameter(_myVelocityShader->createFragmentParameter(_myShaderTypeName), 
            _myVelocityShader->fragmentParameter(_myParameterIndex));

    setupParameter(0, 0);
    _myVelocityShader->checkCGError("Problem creating force.");
    
    std::stringstream ss1;
    ss1 << _myParameterIndex << ".force1";
    
    std::stringstream ss2;
    ss2 << _myParameterIndex << ".force2";
    
    _myForce1->setShader(theParticles, theShader, ss1.str(), theWidth, theHeight);
    _myForce2->setShader(theParticles, theShader, ss2.str(), theWidth, theHeight);
}

void
GPUYForceBlend::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myY1Parameter = parameter("y1");
    _myY2Parameter = parameter("y2");
    _myBlendWidthParameter = parameter("blendWidth");
}

void
GPUYForceBlend::setSize(int theWidth, int theHeight) {
    _myForce1->setSize(theWidth, theHeight);
    _myForce2->setSize(theWidth, theHeight);
}

void
GPUYForceBlend::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    
    _myVelocityShader->parameter(_myY1Parameter, _myY1->getValue<float>());
    _myVelocityShader->parameter(_myY2Parameter, _myY2->getValue<float>());
    _myVelocityShader->parameter(_myBlendWidthParameter, _myBlendWidth->getValue<float>());
    
    _myForce1->update(theDeltaTime);
    _myForce2->update(theDeltaTime);
}

