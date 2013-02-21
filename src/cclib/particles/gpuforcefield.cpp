
#include "gpuforcefield.h"

using namespace cclib; 

GPUForceField::GPUForceField(float theNoiseScale, float theStrength, Vector3f theNoiseOffset) :
    GPUForce("NoiseForceField"), 
    _myNoiseScale(Property_<float>::create("noiseScale", theNoiseScale)), 
    _myNoiseOffset(Property_<Vector3f>::create("noiseOffset", theNoiseOffset)) 
{
    registerProperty(_myNoiseScale);
    registerProperty(_myNoiseOffset);
    setStrength(theStrength);
}

GPUForceFieldPtr
GPUForceField::create(float theNoiseScale, float theStrength, Vector3f theNoiseOffset) {
    return GPUForceFieldPtr(new GPUForceField(theNoiseScale, theStrength, theNoiseOffset));
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
    _myVelocityShader->parameter(_myNoiseOffsetParameter, getNoiseOffset());
    _myVelocityShader->parameter(_myNoiseScaleParameter, getNoiseScale());
}

void 
GPUForceField::setNoiseOffset(Vector3f theNoiseOffset){
    _myNoiseOffset->setValue<Vector3f>(theNoiseOffset);
}

Vector3f 
GPUForceField::getNoiseOffset(){
    return _myNoiseOffset->getValue<Vector3f>();
}

void 
GPUForceField::setNoiseScale(float theNoiseScale){
    _myNoiseScale->setValue<float>(theNoiseScale);
}

float 
GPUForceField::getNoiseScale(){
    return _myNoiseScale->getValue<float>();
}

