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

using namespace unity_plugin;
using namespace cclib;

ParticlesWrapper::ParticlesWrapper():
    _forces(),
    _particleSystem(),
    _gltex(0),
    _componentMap()
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
   
    _forces.push_back(GPUGravity::create());
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
    
    _myEmitter = GPUIndexParticleEmitter::create(_particleSystem);
    _particleSystem->addEmitter(_myEmitter);
    
    Graphics::smooth();
    Graphics::checkError();
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

void
ParticlesWrapper::addAnimation(const std::string & animationType) {
}

void
ParticlesWrapper::addEmitter() {
}

void
ParticlesWrapper::updateSimulation() {
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    // emit new particles
    for(int x = 0; x < 25; x++){
        for(int y = 0; y < 25; y++){
            Vector3f pos = Vector3f(cclib::random<float>(0, 100),
                                    cclib::random<float>(0, 100),
                                    cclib::random<float>(0, 100));
            
            Vector3f vel;
            vel.randomize(0.0f);
            Color c(1.0f, 0.0f, 0.0f, 1.0f);
            _myEmitter->emit(c, pos, vel, 100000, false);
        }
    }
    
    cclib::Graphics::noTexture();
    _particleSystem->update(1.0f/60.0f);
    
    glEnable(GL_POINT_SMOOTH);
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

