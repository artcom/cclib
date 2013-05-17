
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
    _myRadiusParameter(0),
    _myUseNoiseTexture(true)
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
GPUCurveField::updateNoise()
{
    if(_myUseNoiseTexture == false) return;
    
    if(_myNoiseData.size() == 0)
    {
        int width = _myParticles->width();
        _myNoiseData = std::vector<unsigned char>(width * 4, 0);
        _myNoiseTexture = Texture2D::create(_myNoiseData, GL_TEXTURE_RECTANGLE, width, 1);
        
    }

    for (int i=0; i<_myNoiseData.size(); i+=4) {
        
        float x1 = i * getScale() + getOffset();
        float x2 = i * getScale() + getOffset() + 100;
        
        float a = cclib::random2d(x1,0, 0,255);
        float b = cclib::random2d(x2,0, 0,255);
        
        _myNoiseData[i] = (int)a;
        _myNoiseData[i+1] = (int)b;
        _myNoiseData[i+2] = 0;
        _myNoiseData[i+3] = 0;
    }

    _myNoiseTexture->data(_myNoiseData);


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
    
    float myOffset = getOffset(); 
    myOffset += theDeltaTime * getSpeed();
    setOffset(myOffset);

    _myVelocityShader->parameter(_myOffsetParameter, getOffset());
    _myVelocityShader->parameter(_myOutputScaleParameter, getOutputScale());
    _myVelocityShader->parameter(_myScaleParameter, getScale());
    _myVelocityShader->parameter(_myRadiusParameter, getRadius());
    _myVelocityShader->parameter(_myPredictionParameter, getPrediction());

    if(_myParticles == NULL || _myUseNoiseTexture == false)
    {
        _myVelocityShader->parameter(_myUseNoiseParameter, 0);
        return;
    }
    
    updateNoise();
    
    _myVelocityShader->parameter(_myUseNoiseParameter, (int)_myUseNoiseTexture);
    
    _myVelocityShader->parameter(_myNoiseTextureSizeParameter, (float)_myNoiseData.size() / 4.f);
    _myVelocityShader->texture(_myNoiseParameter, _myNoiseTexture->id());
    

//    printf("%s \t%2.2f, %2.2f, %2.2f, %2.2f, %2.2f\n",__PRETTY_FUNCTION__,getOffset(),getOutputScale(),getScale(),getRadius(),getPrediction());
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


