
#include "gpugravity.h"

using namespace cclib; 

GPUGravity::GPUGravity(Vector3fPtr theGravity) :
            GPUForce("Gravity"),
            _myGravityParameter(0),
            _myGravity(theGravity) 
{
}

GPUGravityPtr
GPUGravity::create(Vector3fPtr theGravity) {
    return GPUGravityPtr(new GPUGravity(theGravity));
}

void 
GPUGravity::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myGravityParameter = parameter("gravity");
}

void 
GPUGravity::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    _myVelocityShader->parameter(_myGravityParameter, _myGravity);
}

Vector3fPtr 
GPUGravity::direction(){
    return _myGravity;
}


