
#include "gpuforcefield.h"

using namespace cclib; 

GPUForceField::GPUForceField(float theNoiseScale, float theStrength, Vector3fPtr theNoiseOffset) :
            GPUForce("NoiseForceField"), _myNoiseScale(theNoiseScale), 
            _myNoiseOffset(theNoiseOffset) 
{
    _myStrength = theStrength;
}

GPUForceFieldPtr
GPUForceField::create(float theNoiseScale, float theStrength, Vector3fPtr theNoiseOffset) {
    return GPUForceFieldPtr(new GPUForceField(theNoiseScale, theStrength, theNoiseOffset));
}

GPUForcePtr 
GPUForceField::getBasePtr() {
    return GPUForcePtr(static_cast<GPUForce*>(this));
}

void 
GPUForceField::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myNoiseScaleParameter = parameter("noiseScale");
    _myNoiseOffsetParameter = parameter("noiseOffset");
    _myNoiseLengthScalesParameter = parameter("noiseLengthScales");
    _myNoiseGainsParameter = parameter("noiseGains");

    std::vector<float> v1; 
    v1.push_back(0.4f);  
    v1.push_back(0.23f);
    v1.push_back(0.11f);
    _myVelocityShader->parameter1(_myNoiseLengthScalesParameter, v1);
    
    std::vector<float> v2; 
    v2.push_back(1.0f);  
    v2.push_back(0.5f);
    v2.push_back(0.25f);
    _myVelocityShader->parameter1(_myNoiseGainsParameter, v2);
}

void 
GPUForceField::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    _myVelocityShader->parameter(_myNoiseOffsetParameter, _myNoiseOffset);
    _myVelocityShader->parameter(_myNoiseScaleParameter, _myNoiseScale);
}

void 
GPUForceField::noiseOffset(Vector3fPtr theNoiseOffset){
    _myNoiseOffset = theNoiseOffset;
}

Vector3fPtr 
GPUForceField::noiseOffset(){
    return _myNoiseOffset;
}

void 
GPUForceField::noiseScale(float theNoiseScale){
    _myNoiseScale = theNoiseScale;
}

float 
GPUForceField::noiseScale(){
    return _myNoiseScale;
}

