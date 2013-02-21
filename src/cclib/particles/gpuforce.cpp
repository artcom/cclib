#include <particles/gpuforce.h>

using namespace cclib;

GPUForce::GPUForce(const std::string & theShaderTypeName) : 
    Component(theShaderTypeName),
    _myShaderTypeName(theShaderTypeName),
    _myParticles(),
    _myStrength(Property_<float>::create("strength", 1.0f)),
    _myStrengthParameter()
{
    registerProperty(_myStrength);
    _myShaderTypeName = theShaderTypeName;
}

void 
GPUForce::setupParameter(int theWidth, int theHeight) {
    _myStrengthParameter = parameter("strength");
}

void 
GPUForce::setShader(GPUParticles * theParticles, GPUUpdateShader * theShader,
        int theIndex, int theWidth, int theHeight) 
{
    std::stringstream ss;
    ss << std::string("forces[");
    ss << theIndex; //add number to the stream
    ss << std::string("]");

    setShader(theParticles, theShader, ss.str(), theWidth, theHeight);
}

void 
GPUForce::setShader(GPUParticles * theParticles, GPUUpdateShader * theShader,
        std::string theIndex, int theWidth, int theHeight) 
{
    _myParticles = GPUParticlesPtr(theParticles);
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    _myParameterIndex = theIndex;
    
    _myVelocityShader->checkError("Problem creating force.");
    
    cgConnectParameter(
        _myVelocityShader->createFragmentParameter(_myShaderTypeName), 
        _myVelocityShader->fragmentParameter(_myParameterIndex)
    );

    setupParameter(theWidth, theHeight);
    _myVelocityShader->checkError("Problem creating force.");
}

void 
GPUForce::setSize(int theWidth, int theHeight) {
}

void 
GPUForce::update(float theDeltaTime) {
    _myVelocityShader->parameter(_myStrengthParameter, _myStrength->getValue<float>());
}

void 
GPUForce::reset() {

}

float
GPUForce::getStrength() {
    return _myStrength->getValue<float>();
}

void 
GPUForce::setStrength(float theStrength) {
    _myStrength->setValue(theStrength);
}

CGparameter 
GPUForce::parameter(const std::string & theName){
    return _myVelocityShader->fragmentParameter(_myParameterIndex+"."+theName);
}


