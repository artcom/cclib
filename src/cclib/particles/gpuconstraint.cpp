
#include <particles/gpuupdateshader.h>
#include "gpuconstraint.h"

using namespace cclib;

GPUConstraint::GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, float theMinimalVelocity) :
    _myShaderTypeName(theShaderTypeName), _myResilience(theResilience), _myFriction(theFriction), _myMinimalVelocity(theMinimalVelocity)
{}

void 
GPUConstraint::setShader(GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight)
{
    _myVelocityShader = theShader;
    
    std::stringstream ss;
    ss << std::string("constraints[");
    ss << theIndex; 
    ss << std::string("]");

    _myParameterIndex = ss.str(); 
    cgConnectParameter(
        _myVelocityShader->createFragmentParameter(_myShaderTypeName), 
        _myVelocityShader->fragmentParameter(_myParameterIndex)
    );

    _myResilienceParameter  = parameter("resilience");
    _myFrictionarameter  = parameter("friction");
    _myMinimalVelocityParameter  = parameter("minimalVelocity");

    resilience(_myResilience);
    friction(_myFriction);
    minimalVelocity(_myMinimalVelocity);

    setupParameter(theWidth, theHeight);
    _myVelocityShader->checkError("Problem creating constrain.");
}
	
CGparameter 
GPUConstraint::parameter(const std::string & theName) {
    std::stringstream ss;
    ss << _myParameterIndex;
    ss << std::string(".");
    ss << theName;

    return _myVelocityShader->fragmentParameter(ss.str());
}

void 
GPUConstraint::resilience(float theResilience) {
    _myVelocityShader->parameter(_myResilienceParameter, theResilience);
}

void 
GPUConstraint::friction(float theFriction) {
    _myVelocityShader->parameter(_myFrictionarameter, 1.0f - theFriction);
}

void 
GPUConstraint::minimalVelocity(float theMinimalVelocity) {
    _myVelocityShader->parameter(_myMinimalVelocityParameter, theMinimalVelocity);
}

void 
GPUConstraint::update(float theDeltaTime) {

}

