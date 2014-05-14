#include <particles/gpuupdateshader.h>
#include "gpuimpulse.h"

using namespace cclib;

GPUImpulse::GPUImpulse(const std::string &theShaderTypeName, float theStrength) {
    _myShaderTypeName = theShaderTypeName;
    _myStrength = theStrength;
}

void 
GPUImpulse::setShader(GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight) {
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    std::stringstream ss;
    ss << std::string("impulses[");
    ss << theIndex;
    ss << std::string("]");

    _myParameterIndex = ss.str(); 

    cgConnectParameter(
            _myVelocityShader->createFragmentParameter(_myShaderTypeName), 
            _myVelocityShader->fragmentParameter(_myParameterIndex)
            );

    _myStrengthParameter  = parameter("strength");

    strength(_myStrength);

    setupParameter(theWidth, theHeight);
    _myVelocityShader->checkCGError("Problem creating constrain.");
}

CGparameter 
GPUImpulse::parameter(const std::string & theName) {
    std::string s = _myParameterIndex + std::string(".") + theName;
    return _myVelocityShader->fragmentParameter(_myParameterIndex+"."+theName);
}

void 
GPUImpulse::strength(float theStrength) {
    _myStrength = theStrength;
}

void 
GPUImpulse::trigger() {
    _myTrigger = true;
}

bool 
GPUImpulse::isTriggering() {
    return _myTrigger;
}

void 
GPUImpulse::update(float theDeltaTime) {
    if(_myTrigger) {
        _myTrigger = false;
        _myVelocityShader->parameter(_myStrengthParameter, _myStrength);
    }else {
        _myVelocityShader->parameter(_myStrengthParameter, 0);
    }
}


