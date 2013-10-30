
#include "gpucurvefield.h"
#include <gl/texture2d.h>
#include <math/math.h>

using namespace cclib; 

GPUCurveField::GPUCurveField() :
    GPUForce("CurveForceFieldFollow"),
    _myPrediction(Property_<float>::create("prediction", 0)), 
    _myOffset(Property_<float>::create("offset", 0)), 
    _myScale(Property_<float>::create("scale", 1)), 
    _myOutputScale(Property_<float>::create("outputScale", 1)),
    _myRadius(Property_<float>::create("radius", 1)), 
    _mySpeed(Property_<float>::create("speed", 1)), 
    _myPredictionParameter(0), 
    _myOffsetParameter(0),
    _myScaleParameter(0), 
    _myOutputScaleParameter(0), 
    _myRadiusParameter(0)
{
    registerProperty(_myPrediction); 
    registerProperty(_myOffset); 
    registerProperty(_myScale); 
    registerProperty(_myOutputScale);
    registerProperty(_myRadius); 
    registerProperty(_mySpeed);
}

GPUCurveFieldPtr
GPUCurveField::create() {
    return GPUCurveFieldPtr(new GPUCurveField());
}

void 
GPUCurveField::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);

    _myPredictionParameter = parameter("prediction");
    _myOffsetParameter = parameter("offset");
    _myScaleParameter = parameter("scale");
    _myOutputScaleParameter = parameter("outputScale");
    _myRadiusParameter = parameter("radius");
    
    _myUseNoiseParameter = parameter("useNoiseTexture");
    _myNoiseParameter = parameter("noiseTexture");
    _myNoiseTextureSizeParameter = parameter("noiseTextureSize");
    
//    printf("%s\n",__PRETTY_FUNCTION__);
}


void 
GPUCurveField::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    
    _myVelocityShader->parameter(_myOffsetParameter, getOffset());
    _myVelocityShader->parameter(_myOutputScaleParameter, getOutputScale());
    _myVelocityShader->parameter(_myScaleParameter, getScale());
    _myVelocityShader->parameter(_myRadiusParameter, getRadius());
    _myVelocityShader->parameter(_myPredictionParameter, getPrediction());
}

void 
GPUCurveField::setPrediction(float thePrediction) {
    _myPrediction->setValue<float>(thePrediction);
}

void 
GPUCurveField::setScale(float theScale) {
    _myScale->setValue<float>(theScale);
}

void
GPUCurveField::setOutputScale(float theOutputScale) {
    _myOutputScale->setValue<float>(theOutputScale);
}

void 
GPUCurveField::setRadius(float theRadius) {
    _myRadius->setValue<float>(theRadius);
}

void 
GPUCurveField::setSpeed(float theSpeed) {
    _mySpeed->setValue<float>(theSpeed);
}

void 
GPUCurveField::setOffset(float theOffset) {
    _myOffset->setValue<float>(theOffset);
}

float 
GPUCurveField::getPrediction() {
    return _myPrediction->getValue<float>();
}
        
float  
GPUCurveField::getScale() {
    return _myScale->getValue<float>();
}

float  
GPUCurveField::getOutputScale() {
    return _myOutputScale->getValue<float>();
}

float  
GPUCurveField::getRadius() {
    return _myRadius->getValue<float>();
}

float  
GPUCurveField::getSpeed() {
    return _mySpeed->getValue<float>();
}

float
GPUCurveField::getOffset() {
    return _myOffset->getValue<float>();
}


