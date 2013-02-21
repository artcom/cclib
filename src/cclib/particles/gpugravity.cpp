
#include "gpugravity.h"

using namespace cclib; 

GPUGravity::GPUGravity(Vector3f theGravity) :
    GPUForce("Gravity"),
    _myGravityParameter(0),
    _myGravity(Property_<Vector3f>::create("gravity", theGravity)) 
{
    registerProperty(_myGravity);
}

GPUGravityPtr
GPUGravity::create(Vector3f theGravity) {
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
    _myVelocityShader->parameter(_myGravityParameter, getDirection());
}

Vector3f
GPUGravity::getDirection() {
    return _myGravity->getValue<Vector3f>();
}

void
GPUGravity::setDirection(Vector3f theGravity) {
    _myGravity->setValue<Vector3f>(theGravity);
}

