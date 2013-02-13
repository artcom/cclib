
#include "gpucurvefield.h"

using namespace cclib; 

GPUCurveField::GPUCurveField() :
    GPUForce("CurveForceFieldFollow"),
    _myPrediction(0), _myOffset(0),_myScale(1), _myOutputScale(1),
    _myRadius(1), _mySpeed(1), _myPredictionParameter(0), _myOffsetParameter(0),
    _myScaleParameter(0), _myOutputScaleParameter(0), _myRadiusParameter(0)
{}

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
}

void 
GPUCurveField::prediction(float thePrediction) {
    _myPrediction = thePrediction;
}

void 
GPUCurveField::scale(float theScale) {
    _myScale = theScale;
}

void
GPUCurveField::outputScale(float theOutputScale) {
    _myOutputScale = theOutputScale;
}

void 
GPUCurveField::radius(float theRadius) {
    _myRadius = theRadius;
}

void 
GPUCurveField::speed(float theSpeed) {
    _mySpeed = theSpeed;
}


void 
GPUCurveField::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
	
    _myOffset += theDeltaTime * _mySpeed;

    _myVelocityShader->parameter(_myOffsetParameter, _myOffset);
    _myVelocityShader->parameter(_myOutputScaleParameter, _myOutputScale);
    _myVelocityShader->parameter(_myScaleParameter, _myScale);
    _myVelocityShader->parameter(_myRadiusParameter, _myRadius);
    _myVelocityShader->parameter(_myPredictionParameter, _myPrediction);

}

