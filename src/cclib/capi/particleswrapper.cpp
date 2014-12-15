//
//  ParticleRendering.cpp
//  cclib_plugin
//
//  Created by Sebastian Hein on 1/14/14.
//  Copyright (c) 2014 artcom. All rights reserved.
//

#include "capi/particleswrapper.h"
#include "capi/capi.h"
#include <cclib.h>
#include <gl/graphics.h>
#include <particles/gpuparticles.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpudummyrenderer.h>
#include <particles/gpugravity.h>
#include <gl/shaderbuffer.h>

// the forces
#include <particles/gpugravity.h>
#include <particles/gpuviscousdrag.h>
#include <particles/gpuforcefield.h>
#include <particles/gpuattractor.h>
#include <particles/gpucurvefield.h>
#include <particles/gpucurveline.h>
#include <particles/gpuyforceblend.h>

// the emitters
#include <particles/gpusimpleemitter.h>
#include <particles/gpuindexparticlecurveemitter.h>
#include <particles/gpucurvelineemitter.h>
#include <particles/gpupermanentblockemitter.h>
#include <particles/gpugbufferemitter.h>

using namespace unity_plugin;
using namespace cclib;

ParticlesWrapper::ParticlesWrapper():
    _forces(), _emitters(), _emittersToCreate(),
    _particleSystem(), _componentMap(),
    _positionTexture(0), _infoTexture(0), _colorTexture(0)
{
}

void 
ParticlesWrapper::setDefaultGraphicsState()
{
    glDisable (GL_CULL_FACE);
    glDisable (GL_LIGHTING);
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    glEnable (GL_DEPTH_TEST);
    glDepthMask (GL_FALSE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_BACK);
}

void
ParticlesWrapper::setColorTexture(void* texturePointer) {
    _colorTexture = (GLuint)(size_t)(texturePointer); 
}

void
ParticlesWrapper::setInfoTexture(void* texturePointer) {
    _infoTexture = (GLuint)(size_t)(texturePointer); 
}

void
ParticlesWrapper::setup(void* texturePointer) {
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    Graphics::checkError();
    setDefaultGraphicsState();
   
    std::vector<GPUConstraintPtr> myConstraints;
    std::vector<GPUImpulsePtr> myImpulses;

    _positionTexture = (GLuint)(size_t)(texturePointer);
    if (!glIsTexture(_positionTexture)) {
        throw cclib::Exception("Texture is invalid");
    }
    
    glBindTexture (GL_TEXTURE_2D, _positionTexture);
    
    int texWidth, texHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    glBindTexture(GL_TEXTURE_2D, 0);
  

    GPUDummyRendererPtr myRenderer = GPUDummyRenderer::create();
    _particleSystem = GPUParticles::create(myRenderer, _forces, myConstraints, myImpulses, texWidth, texHeight);
   
    // emitters have to be created using the particle system 
    createEmitters(texWidth, texHeight);

    // register emitters
    for (unsigned int i=0; i<_emitters.size(); i++) {
        _particleSystem->addEmitter(_emitters[i]);
    }
    
    Graphics::smooth();
    Graphics::checkError();
}

void
ParticlesWrapper::createEmitters(unsigned int texWidth, unsigned int texHeight) {
    for (unsigned int i=0; i<_emittersToCreate.size(); i++) {
        std::string key = _emittersToCreate[i];
        std::vector<std::string> typeName;
    
        std::istringstream f(key);
        std::string s;    
        while (std::getline(f, s, ':')) {
            typeName.push_back(s);
        }
       
        if (typeName.size() == 2) {
            GPUIndexParticleEmitterPtr emitter = createEmitterFromString(typeName[0], texWidth, texHeight);
            _emitters.push_back(emitter);
            _componentMap[typeName[1]] = emitter;
        }
    }
}

GPUForcePtr
ParticlesWrapper::createForceFromString(const std::string & forceType) {
    if (forceType == "gravity") return GPUGravity::create();
    if (forceType == "forcefield") return GPUForceField::create();
    if (forceType == "viscousdrag") return GPUViscousDrag::create();
    if (forceType == "attractor") return GPUAttractor::create();
    if (forceType == "curvefield") return GPUCurveField::create();
    if (forceType == "curveline") return GPUCurveLine::create();
    if (forceType == "yforceblend") return GPUYForceBlend::create();
    
    throw new cclib::Exception("unknown force type.");
}
    
void 
ParticlesWrapper::addCombinedForce(const std::string & forceType, std::string & identifier, 
        std::string & force1, std::string & force2) 
{
    GPUForcePtr force = createForceFromString(forceType);
    
    // initialize forces the are blending other nested forces, like YForceBlend (the only one for now)
    GPUForcePtr f1;
    GPUForcePtr f2;

    if (_componentMap.find(force1) != _componentMap.end()) {
        f1 = CC_DYN_PTR_CAST<GPUForce>(_componentMap[force1]);     
    }
    if (_componentMap.find(force2) != _componentMap.end()) {
        f2 = CC_DYN_PTR_CAST<GPUForce>(_componentMap[force2]);     
    }

    GPUYForceBlendPtr blendPtr = CC_DYN_PTR_CAST<GPUYForceBlend>(force); 

    blendPtr->initialize(f1, f2); 
   
    std::vector<GPUForcePtr>::iterator pos = std::find(_forces.begin(), _forces.end(), f1);
    if (pos != _forces.end()) {
        _forces.erase(pos); 
    }

    pos = std::find(_forces.begin(), _forces.end(), f2);
    if (pos != _forces.end()) {
        _forces.erase(pos); 
    }

    // add the force to the component map to be able to access the parameters later
    _componentMap[identifier] = force;
    
    // add the force to the initialization list
    _forces.push_back(force);
}

void
ParticlesWrapper::addForce(const std::string & forceType, std::string & identifier) {
    GPUForcePtr force = createForceFromString(forceType);

    // add the force to the component map to be able to access the parameters later
    _componentMap[identifier] = force;
    
    // add the force to the initialization list
    _forces.push_back(force);
}

GPUIndexParticleEmitterPtr
ParticlesWrapper::createEmitterFromString(const std::string & emitterType, unsigned int texWidth, unsigned int texHeight) {
    if (emitterType == "simpleemitter") return GPUSimpleEmitter::create(_particleSystem, 0, texWidth*texHeight);
    if (emitterType == "curvelineemitter") return GPUCurveLineEmitter::create(_particleSystem, 0, texWidth*texHeight);
    if (emitterType == "indexparticlecurveemitter") return GPUIndexParticleCurveEmitter::create(_particleSystem, 0, texWidth*texHeight);
    if (emitterType == "permanentblockemitter") return GPUPermanentBlockEmitter::create(_particleSystem, 0, texWidth, texHeight);
    if (emitterType == "gbufferemitter") return GPUGBufferEmitter::create(_particleSystem);

    throw new cclib::Exception("unknown emitter type.");
}
            
void
ParticlesWrapper::addEmitter(const std::string & emitterType, std::string & identifier) {
    std::string emitterKey = emitterType + std::string(":") + identifier;
    _emittersToCreate.push_back(emitterKey);
}

void
ParticlesWrapper::addAnimation(const std::string & animationType) {
}

void
ParticlesWrapper::updateSimulation(float theDeltaT) {
    
    setDefaultGraphicsState();

    Graphics::checkError();
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    glDisable(GL_POLYGON_SMOOTH);
    Graphics::checkError();
    
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    Graphics::checkError();
   
    cclib::Graphics::noTexture();
    _particleSystem->update(theDeltaT);
    Graphics::checkError();
    
    glEnable(GL_POINT_SMOOTH);
    glEnable (GL_DEPTH_TEST);
    glDepthMask (GL_TRUE);
    Graphics::checkError();
}
            
void
ParticlesWrapper::copyResults() {
    setDefaultGraphicsState();
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
   
    _particleSystem->dataBuffer()->bindFBO(0);
    glBindTexture (GL_TEXTURE_2D, _positionTexture);

    int texWidth, texHeight;
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth, texHeight);
    
    glBindTexture (GL_TEXTURE_2D, 0);
    _particleSystem->dataBuffer()->releaseFBO();
    
    Graphics::checkError();

    if (_infoTexture != 0) {
        _particleSystem->dataBuffer()->bindFBO(1);
        glBindTexture (GL_TEXTURE_2D, _infoTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth, texHeight);
        glBindTexture (GL_TEXTURE_2D, 0);
        _particleSystem->dataBuffer()->releaseFBO();
    }
    
    Graphics::checkError();
    
    if (_colorTexture != 0) {
        _particleSystem->dataBuffer()->bindFBO(3);
        glBindTexture (GL_TEXTURE_2D, _colorTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth, texHeight);
        glBindTexture (GL_TEXTURE_2D, 0);
        _particleSystem->dataBuffer()->releaseFBO();
    }
    
    Graphics::checkError();
}

