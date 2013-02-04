#include <particles/gpuforce.h>

using namespace cclib;

GPUForce::GPUForce(const std::string & theShaderTypeName) :
    _myShaderTypeName(theShaderTypeName),
    _myStrength(1.0f),
    _myStrengthParameter(),
    _myVelocityShader(),
    _myParameterIndex(),
    _myParticles()
{
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
    std::cout << ss.str() << std::endl;

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
    //		update(0);
    _myVelocityShader->checkError("Problem creating force.");
}

void 
GPUForce::setSize(int theWidth, int theHeight) {
}

void 
GPUForce::update(float theDeltaTime) {
    _myVelocityShader->parameter(_myStrengthParameter, _myStrength);
}

void 
GPUForce::reset() {

}

void 
GPUForce::strength(float theStrength) {
    _myStrength = theStrength;
}

CGparameter 
GPUForce::parameter(const std::string & theName){
    return _myVelocityShader->fragmentParameter(_myParameterIndex+"."+theName);
}


