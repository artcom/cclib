
#include "gputimeforceblend.h"
#include <gl/texture2d.h>

using namespace cclib; 

int GPUTimeForceBlend::MAX_STATES = 10;

GPUTimeForceBlend::GPUTimeForceBlend() : 
    GPUForce("TimeForceBlend"),
    _myStartTime(Property_<float>::create("startTime", 0.0f)),
    _myEndTime(Property_<float>::create("endTime", 0.0f)),
    _myPower(Property_<float>::create("power", 0.0f))
{
    registerProperty(_myStartTime);
    registerProperty(_myEndTime);
    registerProperty(_myPower);
    
    _myBlendInfoData = std::vector<unsigned char>(GPUTimeForceBlend::MAX_STATES * 4, 0);
    for (int i=0; i<GPUTimeForceBlend::MAX_STATES; i+=4) {
        _myBlendInfoData[i  ] = 0;
        _myBlendInfoData[i+1] = 255;
        _myBlendInfoData[i+2] = 0;
        _myBlendInfoData[i+3] = 0;
    }

    _myBlendInfos = Texture2D::create(_myBlendInfoData, GL_TEXTURE_RECTANGLE, GPUTimeForceBlend::MAX_STATES, 1);
    _myBlendInfos->data(_myBlendInfoData);
}

GPUTimeForceBlend::GPUTimeForceBlend(float theStartTime,float theEndTime,GPUForcePtr theForce1,GPUForcePtr theForce2)
: GPUForce("TimeForceBlend")
{
    GPUTimeForceBlend::GPUTimeForceBlend();
    
    _myStartTime->set(theStartTime);
    _myEndTime->set(theEndTime);
    
    _myForce1 = theForce1;
    _myForce2 = theForce2;
}

void
GPUTimeForceBlend::initialize(GPUForcePtr theForce1, GPUForcePtr theForce2) 
{
    _myForce1 = theForce1;
    _myForce2 = theForce2;
}

GPUTimeForceBlendPtr
GPUTimeForceBlend::create() {
    return GPUTimeForceBlendPtr(new GPUTimeForceBlend());
}

GPUTimeForceBlendPtr
GPUTimeForceBlend::create(float theStartTime,float theEndTime,GPUForcePtr theForce1,GPUForcePtr theForce2) {
    return GPUTimeForceBlendPtr(new GPUTimeForceBlend(theStartTime, theEndTime, theForce1, theForce2));
}

void
GPUTimeForceBlend::setShader(GPUParticles * theParticles, GPUUpdateShader * theShader, int theIndex, int theWidth, int theHeight) {
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    
    std::stringstream ss;
    ss << "forces[" << theIndex << "]";
    
    _myParameterIndex = ss.str();

    cgConnectParameter( _myVelocityShader->createFragmentParameter(_myShaderTypeName), _myVelocityShader->fragmentParameter(_myParameterIndex));

    setupParameter(theWidth, theHeight);
    _myVelocityShader->checkCGError("Problem creating force.");
    
    std::stringstream ss1;
    ss1 << _myParameterIndex << ".force1";

    std::stringstream ss2;
    ss2 << _myParameterIndex << ".force2";
    
    _myForce1->setShader(theParticles, theShader, ss1.str(), theWidth, theHeight);
    _myForce2->setShader(theParticles, theShader, ss2.str(), theWidth, theHeight);
}

void
GPUTimeForceBlend::setShader(GPUParticles * theParticles, GPUUpdateShader * theShader, std::string theIndex, int theWidth, int theHeight) {
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    _myParameterIndex = theIndex;
    _myVelocityShader->checkCGError("Problem creating force.");
    
    cgConnectParameter(_myVelocityShader->createFragmentParameter(_myShaderTypeName), _myVelocityShader->fragmentParameter(_myParameterIndex));

    setupParameter(0, 0);
    _myVelocityShader->checkCGError("Problem creating force.");
    
    std::stringstream ss1;
    ss1 << _myParameterIndex << ".force1";
    
    std::stringstream ss2;
    ss2 << _myParameterIndex << ".force2";
    
    _myForce1->setShader(theParticles, theShader, ss1.str(), theWidth, theHeight);
    _myForce2->setShader(theParticles, theShader, ss2.str(), theWidth, theHeight);
}

void
GPUTimeForceBlend::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myStartTimeParameter  = parameter("start");
    _myEndTimeParameter    = parameter("end");
    _myBlendInfosParameter = parameter("blendInfos");
    _myPowerParameter      = parameter("power");
}

void
GPUTimeForceBlend::setSize(int theWidth, int theHeight) {
    _myForce1->setSize(theWidth, theHeight);
    _myForce2->setSize(theWidth, theHeight);
}

void
GPUTimeForceBlend::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    
    _myVelocityShader->parameter(_myStartTimeParameter, _myStartTime->getValue<float>());
    _myVelocityShader->parameter(_myEndTimeParameter, _myEndTime->getValue<float>());
    
    _myVelocityShader->texture(_myBlendInfosParameter, _myBlendInfos->id());
    
    _myVelocityShader->parameter(_myPowerParameter, _myPower->getValue<float>());
    
    _myForce1->update(theDeltaTime);
    _myForce2->update(theDeltaTime);
}

void
GPUTimeForceBlend::setBlend(int theState, float theMinBlend, float theMaxBlend) {

    float r = theMinBlend;
    float g = theMaxBlend;
    
    int index = theState * 4;
    _myBlendInfoData[index + 0] = r * 255;
    _myBlendInfoData[index + 1] = g * 255;
    _myBlendInfoData[index + 2] = 0;
    _myBlendInfoData[index + 3] = 0;
    
    _myBlendInfos->data(_myBlendInfoData);
}

void
GPUTimeForceBlend::setBlend(float theMinBlend, float theMaxBlend) {
    setBlend(0, theMinBlend, theMaxBlend);
}

