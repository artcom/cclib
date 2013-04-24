
#include "gputargetforce.h"
#include <stringified_shaders/initvalue01.fp.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>
#include <particles/gputargetsetup.h>
#include <particles/gpuindexparticleemitter.h>

using namespace cclib; 

GPUTargetForce::GPUTargetForce(int theTargetTextures) :
    GPUForce("TargetForce"),
    _myInitialTargetTextures(theTargetTextures),
    _myTargetPositionTextures(),
    _myInitValueShader(),
    _myWidth(0),
    _myHeight(0),
    _myCurrentIndex(0),

	// _myTargetPositionTexture("targetPositionTexture");
    _myScale(Property_<float>::create("scale", 1.0f)),
    _myCenter(Property_<Vector3f>::create("center", Vector3f())),
    _myLookAhead(Property_<float>::create("lookAhead", 0.0f)),
    _myMaxForce(Property_<float>::create("maxForce", 0.0f)),
    _myNearDistance(Property_<float>::create("nearDistance", 0.0f)),
    _myNearMaxForce(Property_<float>::create("nearMaxForce", 0.0f))
{
    registerProperty(_myScale);
    registerProperty(_myScale);
    registerProperty(_myCenter);
    registerProperty(_myLookAhead);
    registerProperty(_myMaxForce);
    registerProperty(_myNearDistance);
    registerProperty(_myNearMaxForce);
}

GPUTargetForcePtr
GPUTargetForce::create(int theTargetTextures) {
    return GPUTargetForcePtr(new GPUTargetForce(theTargetTextures));
}

void 
GPUTargetForce::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
	_myTargetPositionTextureParameter = parameter("targetPositionTexture");
    _myCenterParameter = parameter("center");
    _myScaleParameter = parameter("scale");
    _myLookAheadParameter = parameter("lookAhead");
    _myMaxForceParameter = parameter("maxForce");
    _myNearDistanceParameter = parameter("nearDistance");
    _myNearMaxForceParameter = parameter("nearMaxForce");
}

void 
GPUTargetForce::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    _myVelocityShader->parameter(_myScaleParameter, _myScale->getValue<float>());
    _myVelocityShader->parameter(_myLookAheadParameter, _myLookAhead->getValue<float>());
    _myVelocityShader->parameter(_myMaxForceParameter, _myMaxForce->getValue<float>());
    _myVelocityShader->parameter(_myCenterParameter, _myCenter->getValue<Vector3f>());
    _myVelocityShader->parameter(_myNearDistanceParameter, _myNearDistance->getValue<float>());
    _myVelocityShader->parameter(_myNearMaxForceParameter, _myNearMaxForce->getValue<float>());
}

void
GPUTargetForce::setSize(int theWidth, int theHeight) {
    _myWidth = theWidth;
    _myHeight = theHeight;
    
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    ffiles.push_back(initvalue01_fp);
    
    _myInitValueShader = Shader::create(vfiles, ffiles);
    _myInitValueShader->load();
    
    for(int i = 0; i < _myInitialTargetTextures; i++) {
        ShaderBufferPtr myTargets = ShaderBuffer::create(theWidth, theHeight, 16, 4);
        myTargets->beginDraw();
        Graphics::clearColor(0, 0, 0, 0);
        Graphics::clear();
        myTargets->endDraw();
        _myTargetPositionTextures.push_back(myTargets);
    }
}

void
GPUTargetForce::setTargets(ShaderBufferPtr theTargetTexture, GPUTargetSetupPtr theSetup,
                           int theX, int theY, int theWidth, int theHeight)
{
    Graphics::pushAttribute();
    Graphics::noBlend();
    theTargetTexture->beginDraw();
    _myInitValueShader->start();
    
    Graphics::beginShape(GL_POINTS);
    theSetup->setParticleTargets(theX, theY, theWidth, theHeight);
    Graphics::endShape();
    
    _myInitValueShader->end();
    theTargetTexture->endDraw();
    Graphics::popAttribute();
}

void
GPUTargetForce::setTargets(ShaderBufferPtr theTargetTexture, GPUTargetSetupPtr theSetup,
                           GPUIndexParticleEmitterPtr theGroup)
{
    Graphics::pushAttribute();
    Graphics::noBlend();
    theTargetTexture->beginDraw();
    _myInitValueShader->start();
    
    Graphics::beginShape(GL_POINTS);
    theSetup->setParticleTargets(theGroup);
    Graphics::endShape();
    
    _myInitValueShader->end();
    theTargetTexture->endDraw();
    Graphics::popAttribute();
}

void
GPUTargetForce::addTargetSetup(GPUTargetSetupPtr theSetup)
{
    ShaderBufferPtr myTexture = ShaderBuffer::create(_myWidth, _myHeight, 16, 4);
    _myTargetPositionTextures.push_back(myTexture);
    
    setTargets(myTexture, theSetup, 0, 0, _myWidth, _myHeight);
    
    if(_myTargetPositionTextures.size() == 1) {
        // Object myObject = _myTargetPositionTextures.get(_myCurrentIndex);
        // if(myObject instanceof Integer) {
        //     _myVelocityShader.texture(_myTargetPositionTextureParameter, (Integer)myObject);
        // }else {
        _myVelocityShader->texture(_myTargetPositionTextureParameter, _myTargetPositionTextures[_myCurrentIndex]->attachment(0)->id());
        // }
    }
}

void
GPUTargetForce::updateSetup(int theIndex, GPUTargetSetupPtr theSetup,
                            int theX, int theY, int theWidth, int theHeight)
{
    ShaderBufferPtr myTexture = _myTargetPositionTextures[theIndex];
    
    setTargets(myTexture, theSetup, theX, theY, theWidth, theHeight);
}

void
GPUTargetForce::updateSetup(int theIndex, GPUTargetSetupPtr theSetup, GPUIndexParticleEmitterPtr theParticleGroup)
{
    ShaderBufferPtr myTexture = _myTargetPositionTextures[theIndex];
    
    setTargets(myTexture, theSetup, theParticleGroup);
}

void
GPUTargetForce::updateSetup(int theIndex, GPUTargetSetupPtr theSetup)
{
    ShaderBufferPtr myTexture = _myTargetPositionTextures[theIndex];
    
    setTargets(myTexture, theSetup, 0, 0, _myWidth, _myHeight);
}

void
GPUTargetForce::addTargetSetup(ShaderBufferPtr theShaderBuffer)
{
    std::cerr << "fix me!" << std::endl;
    _myTargetPositionTextures.push_back(theShaderBuffer);
    if(_myTargetPositionTextures.size() == 1) {
        // Object myObject = _myTargetPositionTextures.get(_myCurrentIndex);
        // if(myObject instanceof Integer) {
        //     _myVelocityShader.texture(_myTargetPositionTextureParameter, (Integer)myObject);
        // }else {
        _myVelocityShader->texture(_myTargetPositionTextureParameter, _myTargetPositionTextures[_myCurrentIndex]->attachment(0)->id());
        // }
    }
}

/* void
GPUTargetForce::addTargetSetup(int theTextureID)
{
    _myTargetPositionTextures.push_back(theTextureID);
    if(_myTargetPositionTextures.size() == 1) {
        // Object myObject = _myTargetPositionTextures.get(_myCurrentIndex);
        // if(myObject instanceof Integer) {
        //     _myVelocityShader.texture(_myTargetPositionTextureParameter, (Integer)myObject);
        // }else {
            _myVelocityShader->texture(_myTargetPositionTextureParameter, _myTargetPositionTextures[_myCurrentIndex]->attachment(0)->id());
        // }
    }
} */

void
GPUTargetForce::changeSetup(int theIndex)
{
    _myCurrentIndex = theIndex;
    // Object myObject = _myTargetPositionTextures.get(_myCurrentIndex);
    // if(myObject instanceof Integer) {
    //     _myVelocityShader.texture(_myTargetPositionTextureParameter, (Integer)myObject);
    // }else {
        _myVelocityShader->texture(_myTargetPositionTextureParameter, _myTargetPositionTextures[_myCurrentIndex]->attachment(0)->id());
    // }
}


