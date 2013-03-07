
#include "gputimeforceblend.h"

using namespace cclib; 
    
GPUAttractor::GPUAttractor(Vector3f thePosition, float theStrength, float theRadius) :
            GPUForce("Attractor"), 
            _myRadius(Property_<float>::create("radius", theRadius)),
            _myPosition(Property_<Vector3f>::create("position", thePosition))
{
    setStrength(theStrength);
    registerProperty(_myRadius);
    registerProperty(_myPosition);
}

GPUAttractorPtr
GPUAttractor::create(Vector3f thePosition, float theStrength, float theRadius) {
    return GPUAttractorPtr(new GPUAttractor(thePosition, theStrength, theRadius));
}

void 
GPUAttractor::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myPositionParameter = parameter("position");
    _myRadiusParameter = parameter("radius");
}

void 
GPUAttractor::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    _myVelocityShader->parameter(_myPositionParameter, getPosition());
    _myVelocityShader->parameter(_myStrengthParameter, getStrength());
    _myVelocityShader->parameter(_myRadiusParameter, getRadius());
}
	
void 
GPUAttractor::setPosition(Vector3f thePosition){
    _myPosition->setValue<Vector3f>(thePosition);
}

Vector3f
GPUAttractor::getPosition() {
    return _myPosition->getValue<Vector3f>();
}

float 
GPUAttractor::getStrength() {
    return _myStrength->getValue<float>();
}

void 
GPUAttractor::setRadius(float theRadius) {
    _myRadius->setValue<float>(theRadius);
}

float 
GPUAttractor::getRadius() {
    return _myRadius->getValue<float>();
}


