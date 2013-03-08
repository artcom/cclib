
#include "gputimeforceblend.h"
#include <gl/texture2d.h>

using namespace cclib; 

int GPUTimeForceBlend::MAX_STATES = 10;

GPUTimeForceBlend::GPUTimeForceBlend(float theStartTime, float theEndTime, GPUForcePtr theForce1, GPUForcePtr theForce2) :
            GPUForce("TimeForceBlend"),
            _myStartTime(theStartTime),
            _myEndTime(theEndTime),
            _myPower(1),
            _myForce1(theForce1),
            _myForce2(theForce2)
{
    _myBlendInfoData = std::vector<unsigned char>(GPUTimeForceBlend::MAX_STATES * 4, 0);
    for (int i=0; i<GPUTimeForceBlend::MAX_STATES; i+=4) {
        _myBlendInfoData[i  ] = 0;
        _myBlendInfoData[i+1] = 255;
        _myBlendInfoData[i+2] = 0;
        _myBlendInfoData[i+3] = 0;
    }

    _myBlendInfos = Texture2D::create(_myBlendInfoData, GL_TEXTURE_RECTANGLE, GPUTimeForceBlend::MAX_STATES, 1);
}

GPUTimeForceBlendPtr
GPUTimeForceBlend::create(float theStartTime, float theEndTime, GPUForcePtr theForce1, GPUForcePtr theForce2) {
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
    _myVelocityShader->checkError("Problem creating force.");
    
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
    _myVelocityShader->checkError("Problem creating force.");
    
    cgConnectParameter(_myVelocityShader->createFragmentParameter(_myShaderTypeName), _myVelocityShader->fragmentParameter(_myParameterIndex));

    setupParameter(0, 0);
    _myVelocityShader->checkError("Problem creating force.");
    
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
    GPUTimeForceBlend::update(theDeltaTime);
    
    _myVelocityShader->parameter(_myStartTimeParameter, _myStartTime);
    _myVelocityShader->parameter(_myEndTimeParameter, _myEndTime);
    
    _myVelocityShader->texture(_myBlendInfosParameter, _myBlendInfos->id());
    
    _myVelocityShader->parameter(_myPowerParameter, _myPower);
    
    _myForce1->update(theDeltaTime);
    _myForce2->update(theDeltaTime);
}

void
GPUTimeForceBlend::startTime(float theStartTime) {
    _myStartTime = theStartTime;
}

void
GPUTimeForceBlend::endTime(float theEndTime) {
    _myEndTime = theEndTime;
}

void
GPUTimeForceBlend::blend(int theState, float theMinBlend, float theMaxBlend) {

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
GPUTimeForceBlend::blend(float theMinBlend, float theMaxBlend) {
    blend(0, theMinBlend, theMaxBlend);
}

void
GPUTimeForceBlend::power(float thePower) {
    _myPower = thePower;
}

