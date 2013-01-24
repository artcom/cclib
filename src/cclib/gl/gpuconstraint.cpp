
#include <gl/gpuconstraint.h>

GPUConstraint::GPUConstraint(const std::string & theShaderTypeName, float theResilience, float theFriction, float theMinimalVelocity) 
{
    _myShaderTypeName = theShaderTypeName;

    _myResilience = theResilience;
    _myFriction = theFriction;
    _myMinimalVelocity = theMinimalVelocity;
}

void 
GPUConstraint::setShader(GPUUpdateShader::Ptr theShader, int theIndex, int theWidth, int theHeight)
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
    _myVelocityShader.checkError("Problem creating constrain.");
}

