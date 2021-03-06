
#include <float.h>

#include "gpuparticles.h"
#include <particles/gpuforce.h>
#include <particles/gpuparticlerenderer.h>
#include <particles/gpuparticle.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuconstraint.h>
#include <particles/gpuimpulse.h>
#include <gl/graphics.h>
#include <gl/cgshader.h>
#include <gl/shaderbuffer.h>
#include <stringified_shaders/initvalue01.fp.h>
#include <stringified_shaders/initvalue.fp.h>

using namespace cclib;

GPUParticles::GPUParticles( GPUParticleRendererPtr theRender,
        std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints, 
        std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight) :
        _myForces(theForces), _myConstraints(theConstraints),
        _myImpulses(theImpulse), _myWidth(theWidth), 
        _myHeight(theHeight), _myCurrentTime(0.0),
        // shared indexemitter stuff
        particleWaitingList(),
        freeIndices(),
        activeParticlesArray(),
        allocatedParticles(),
        deadParticles(),
        pendingParticles(),
        stateChanges()
{
    freeIndices = std::vector<int>(theWidth * theHeight, 0);
    particleWaitingList = ParticleWaitingList::create(0.5f);
    activeParticlesArray = std::vector<GPUParticlePtr>();

};

void
GPUParticles::teardown() {
    _myForces.clear();
    _myImpulses.clear();
    _myConstraints.clear();
    freeIndices.clear();
}

GPUParticles::~GPUParticles() {
    std::cout << " -- destroying particles" << std::endl;
    teardown();
}

void GPUParticles::setup(GPUParticlesPtr theThis, GPUParticleRendererPtr theRender,
            std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints,
            std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight)
{
    for(int i = 0; i < theWidth * theHeight; i++) {
        int myIndex = /*_myStart +*/ i;
        activeParticlesArray.push_back(GPUParticle::create(theThis, myIndex));
        freeIndices[i] = myIndex;
    }
    
    for (std::vector<GPUForcePtr>::size_type f=0; f<theForces.size(); f++) {
        theForces[f]->setSize(theWidth, theHeight);
    }

    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(std::string(initvalue01_fp));

    _myInitValue01Shader = CGShader::create(vfiles, ffiles, "main", "main");
    _myInitValue01Shader->load();

    ffiles.clear();
    ffiles.push_back(std::string(initvalue_fp));
    _myInitValue0Shader = CGShader::create(vfiles, ffiles);
    _myInitValue0Shader->load();

    _myCurrentDataTexture = ShaderBuffer::create( _myWidth, _myHeight, 32, 4, 4);
    Graphics::clearColor(0.0f, 0.0f, 1.0f);
    _myCurrentDataTexture->beginDraw(0);
    Graphics::clear();
    _myCurrentDataTexture->endDraw();
    _myCurrentDataTexture->beginDraw(1);
    Graphics::clear();
    _myCurrentDataTexture->endDraw();
    Graphics::clearColor(0);

    _myDestinationDataTexture = ShaderBuffer::create(_myWidth, _myHeight, 32, 4, 4);

    Graphics::noBlend();
    _myCurrentDataTexture->beginDraw();
    _myInitValue01Shader->start();

    Graphics::beginShape(GL_POINTS);
    for (int i = 0; i < _myWidth * _myHeight; i++) {
        Graphics::textureCoords(0, 0.0f, 0.0f, 0.0f);
        Graphics::textureCoords(1, 0.0f, 0.0f, 0.0f);
        Graphics::vertex(i % _myWidth, i / _myWidth);
    }
    Graphics::endShape();

    _myInitValue01Shader->end();
    _myCurrentDataTexture->endDraw();

    _myParticleRender = theRender;
    _myParticleRender->setup( theThis );
    
    std::vector<std::string> myNoShaders;
    _myUpdateShader = GPUUpdateShader::create( theThis, theForces, theConstraints, theImpulse, myNoShaders, _myWidth, _myHeight );

    reset();
}
    
GPUParticlesPtr 
GPUParticles::create(GPUParticleRendererPtr theRender,
        std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints, 
        std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight) 
{
    GPUParticlesPtr obj =  GPUParticlesPtr(new GPUParticles(theRender, theForces, theConstraints, theImpulse, theWidth, theHeight));
    obj->setup(obj, theRender, theForces, theConstraints, theImpulse, theWidth, theHeight);
    
    return obj;
}

void 
GPUParticles::addEmitter(GPUParticleEmitterPtr theEmitter) {
    _myEmitter.push_back(theEmitter);
}

CGShaderPtr 
GPUParticles::initValueShader() {
    return _myInitValue01Shader;
}

double 
GPUParticles::currentTime() {
    return _myCurrentTime;
}

void 
GPUParticles::reset(){
    _myCurrentDataTexture->clear();

    _myCurrentDataTexture->beginDraw();
    _myInitValue01Shader->start();

    Graphics::beginShape(GL_POINTS);
    for (int i = 0; i < _myWidth * _myHeight; i++){
        Graphics::textureCoords(0, FLT_MAX, FLT_MAX, FLT_MAX);
        Graphics::textureCoords(1, 1, 1, 1);
        Graphics::textureCoords(2, 0, 0, 0);
        Graphics::textureCoords(3, 1, 1, 1);
        Graphics::vertex(i % _myWidth,i / _myWidth);
    }
    Graphics::endShape();

    _myInitValue01Shader->end();
    _myCurrentDataTexture->endDraw();

    for (unsigned int i=0; i<_myForces.size(); i++) {
        _myForces[i]->reset();
    }
}

int
GPUParticles::width() {
    return _myWidth;
}

int 
GPUParticles::height() {
    return _myHeight;
}

int 
GPUParticles::size() {
    return _myWidth * _myHeight;
}

ShaderBufferPtr
GPUParticles::dataBuffer() {
    return _myCurrentDataTexture;
}

Vector3fPtr 
GPUParticles::position(GPUParticlePtr theParticle) {
    std::vector<float> myResult = _myCurrentDataTexture->getData(theParticle->x(), theParticle->y(), 1, 1);
    return Vector3fPtr(new Vector3f(myResult[0], myResult[1], myResult[2]));
}

ShaderBufferPtr 
GPUParticles::destinationDataTexture() {
    return _myDestinationDataTexture;
}

void 
GPUParticles::setPosition(int theIndex, Vector3fPtr thePosition) {
    _myPositionUpdates[theIndex] = thePosition;
}

void 
GPUParticles::updateLifecyle(GPUParticlePtr theParticle) {
    _myLifetimeUpdates.push_back(theParticle);
}

void 
GPUParticles::updateManualPositionChanges() {

    if (_myPositionUpdates.size() == 0) {
        return;
    }

    // Render manually changed positions into the texture.
    _myCurrentDataTexture->beginDraw(0);
    _myInitValue0Shader->start();

    Graphics::beginShape(GL_POINTS);

    std::map<int, Vector3fPtr>::const_iterator itr;

    for(itr = _myPositionUpdates.begin(); itr != _myPositionUpdates.end(); ++itr){
        Graphics::textureCoords(0, (*itr).second);
        Graphics::vertex((*itr).first % _myWidth,  (*itr).first / _myWidth);
    }

    Graphics::endShape();

    _myInitValue0Shader->end();
    _myCurrentDataTexture->endDraw();

    _myPositionUpdates.clear();
}

void 
GPUParticles::initializeNewParticles() {
    // Render current position into texture.

    for (unsigned int i=0; i<_myEmitter.size(); i++) {
        _myEmitter[i]->setData();
    }
}

void 
GPUParticles::changeStates() {
    // XXX originally commented out
    //		_myCurrentDataTexture.beginDraw(1);
    //		_myInitValue0Shader.start();
    //		Graphics::beginShape(CCDrawMode.POINTS);
    //		for(CCGPUParticleEmitter myEmitter:_myEmitter) {
    //			for (CCGPUParticle myChangedParticle:myEmitter.stateChangedParticles()){
    //				Graphics::textureCoords(0, myChangedParticle.age(), myChangedParticle.lifeTime(), myChangedParticle.isPermanent() ? 1 : 0, myChangedParticle.step());
    //				Graphics::vertex(myChangedParticle.x(),myChangedParticle.y());
    //			}
    //			myEmitter.stateChangedParticles().clear();
    //		}
    //		Graphics::endShape();
    //		
    //		_myInitValue0Shader.end();
    //		_myCurrentDataTexture.endDraw();
}

void 
GPUParticles::beforeUpdate() {
    initializeNewParticles();
    Graphics::checkError();
    changeStates();
    Graphics::checkError();
}

void 
GPUParticles::cleanUpParticles() {
    // XXX originally commented out
    //		if(_myActiveParticles.size() <= 0)
    //			return;
    //		
    //		_myCurrentPositionTexture.beginDraw(1);
    //		_myInitValue1Shader.start();
    //		Graphics::beginShape(CCDrawMode.POINTS);
    //				
    //		while (_myActiveParticles.peek() != null && _myActiveParticles.peek().timeOfDeath() < _myCurrentTime){
    //			CCGPUParticle myParticle = _myActiveParticles.poll();
    //			if(myParticle.index == -1) continue;
    //			_myAvailableIndices.add(myParticle.index);
    //			_myActiveParticlesArray[myParticle.index].index = -1;
    //			
    //			Graphics::textureCoords(0, Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
    //			Graphics::textureCoords(1, 0, 0, 1, 0);
    //			Graphics::vertex(myParticle.x() + 0.5f, myParticle.y() + 0.5f);
    //		}
    //		
    //		Graphics::endShape();
    //		_myInitValue1Shader.end();
    //		_myCurrentPositionTexture.endDraw();
}

void 
GPUParticles::afterUpdate(){
    updateManualPositionChanges();
    cleanUpParticles();
}

void 
GPUParticles::update(float theDeltaTime) {
    if(theDeltaTime <= 0) {
        return;
    }

    Graphics::checkError();

    for (unsigned int e=0; e<_myEmitter.size(); e++) {
        _myEmitter[e]->update(theDeltaTime);
    }

    Graphics::checkError();
    Graphics::noBlend();
    
    Graphics::checkError();
    beforeUpdate();

    for (unsigned int f=0; f<_myForces.size(); f++) {
        _myForces[f]->update(theDeltaTime);
    }

    for (unsigned int c=0; c<_myConstraints.size(); c++) {
        _myConstraints[c]->update(theDeltaTime);
    }

    for (unsigned int i=0; i<_myImpulses.size(); i++) {
        _myImpulses[i]->update(theDeltaTime);
    }
    _myUpdateShader->data(_myCurrentDataTexture);
    Graphics::checkError();
    
    _myUpdateShader->deltaTime(theDeltaTime);
    Graphics::checkError();

    _myUpdateShader->start();
    Graphics::checkError();
    
    _myDestinationDataTexture->draw();
    Graphics::checkError();

    _myUpdateShader->end();
    Graphics::checkError();

    swapDataTextures();
    Graphics::checkError();

    afterUpdate();
    _myCurrentTime += theDeltaTime;
    _myParticleRender->update(theDeltaTime);
    
    Graphics::checkError();
    Graphics::blend();
    Graphics::checkError();
}

void 
GPUParticles::swapDataTextures() {
    ShaderBufferPtr myTemp = _myDestinationDataTexture;
    _myDestinationDataTexture = _myCurrentDataTexture;
    _myCurrentDataTexture = myTemp;
}

void 
GPUParticles::draw() {
    _myParticleRender->draw();
}

GPUParticleRendererPtr 
GPUParticles::renderer() {
    return _myParticleRender;
}


