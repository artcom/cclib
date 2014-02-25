//
//  ParticleRendering.cpp
//  cclib_plugin
//
//  Created by Sebastian Hein on 1/14/14.
//  Copyright (c) 2014 artcom. All rights reserved.
//

#include "capi/particleswrapper.h"
#include <cclib.h>
#include <gl/graphics.h>
#include <particles/gpuparticles.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuparticlepointrenderer.h>
#include <particles/gpugravity.h>
#include <gl/shaderbuffer.h>

// the forces
#include <particles/gpugravity.h>
#include <particles/gpuviscousdrag.h>
#include <particles/gpuforcefield.h>
#include <particles/gpuattractor.h>
#include <particles/gpucurvefield.h>

// the emitters
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuindexparticlecurveemitter.h>

using namespace unity_plugin;
using namespace cclib;

ParticlesWrapper::ParticlesWrapper():
    _forces(), _emitters(), _emittersToCreate(),
    _particleSystem(), _gltex(0), _componentMap()
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
    
    _gltex = (GLuint)(size_t)(texturePointer);
    glBindTexture (GL_TEXTURE_2D, _gltex);
    
    int texWidth, texHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
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
    
    throw new cclib::Exception("unknown force type.");
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
    if (emitterType == "indexparticleemitter") return GPUIndexParticleEmitter::create(_particleSystem, 0, texWidth*texHeight);
    if (emitterType == "indexparticlecurveemitter") return GPUIndexParticleCurveEmitter::create(_particleSystem, 0, texWidth*texHeight);
    
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
ParticlesWrapper::updateSimulation() {
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
   
    // GPUIndexParticleCurveEmitterPtr e = std::tr1::dynamic_pointer_cast<GPUIndexParticleCurveEmitter>(_emitters[0]);
    // e->update(1.0f/60.0f);
    
    cclib::Graphics::noTexture();
    _particleSystem->update(1.0f/60.0f);
    
    glEnable(GL_POINT_SMOOTH);
    glEnable (GL_DEPTH_TEST);
    glDepthMask (GL_TRUE);
}
            
void
ParticlesWrapper::copyResults() {
    glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    
    _particleSystem->dataBuffer()->bindFBO(0);
    glBindTexture (GL_TEXTURE_2D, _gltex);
    
    int texWidth, texHeight;
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth, texHeight);
    
    glBindTexture (GL_TEXTURE_2D, 0);
    _particleSystem->dataBuffer()->releaseFBO();
}

