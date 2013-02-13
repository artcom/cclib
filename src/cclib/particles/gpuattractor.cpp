
#include "gpuattractor.h"

using namespace cclib; 
    
void position(Vector3fPtr thePosition);
Vector3fPtr position();
float strength();
void radius(float theRadius);
float radius();

GPUAttractor::GPUAttractor(Vector3fPtr thePosition, float theStrength, float theRadius) :
            GPUForce("Attractor"), 
            _myRadius(theRadius)
{
    _myStrength = theStrength;
    _myPosition = Vector3fPtr(new Vector3f(thePosition->x(), thePosition->y(), thePosition->z())); 
}

GPUAttractorPtr
GPUAttractor::create(Vector3fPtr thePosition, float theStrength, float theRadius) {
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
    _myVelocityShader->parameter(_myPositionParameter, _myPosition);
    _myVelocityShader->parameter(_myStrengthParameter, _myStrength);
    _myVelocityShader->parameter(_myRadiusParameter, _myRadius);
}
	
void 
GPUAttractor::position(Vector3fPtr thePosition){
    _myPosition = thePosition;
}

Vector3fPtr 
GPUAttractor::position() {
    return _myPosition;
}

float 
GPUAttractor::getStrength() {
    return _myStrength;
}

void 
GPUAttractor::radius(float theRadius) {
    _myRadius = theRadius;
}

float 
GPUAttractor::radius() {
    return _myRadius;
}


