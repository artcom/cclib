//
//  gpuindexparticlecurveemitter.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gputargetemitter.h"

#include "capi/capi.h"
#include "capi/particleswrapper.h"
#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "stringified_shaders/initvalue01.fp.h"
#include "gl/bufferobject.h"
#include "math/math.h"
#include "particles/gpuparticle.h"
#include "particles/gputargetforce.h"
#include "gl/cgshader.h"
#include "gl/shaderbuffer.h"
#include "particles/gpuparticles.h"
#include "gl/mesh.h"
#include "gl/graphics.h"

#define MAX(x, y) ((y > x)?y:x)  

cclib::GPUTargetEmitter::GPUTargetEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) : 
    cclib::GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles),
    _myNumberOfParticles(theNumberParticles),
    
    // emitting properties
    _myParticlesPerFrame(Property_<float>::create("particlesPerFrame", 1000.0f)),
    _myPosition(Property_<cclib::Vector3f>::create("position", Vector3f(0,0,0))),
    _myLastPosition(Property_<cclib::Vector3f>::create("lastPosition", Vector3f(0,0,0))),
    _myPositionSpread(Property_<cclib::Vector3f>::create("positionSpread", Vector3f(0,0,0))),
    _myTargetPositionSpread(Property_<cclib::Vector3f>::create("targetPositionSpread", Vector3f(0,0,0))),
    _myMaxVelocity(Property_<float>::create("maxVelocity", 0.0f)),
    _myLifetime(Property_<float>::create("lifetime", 10000)),
    _myLifetimeSpread(Property_<float>::create("lifetimeSpread", 100)),
    _myTargetForceName(Property_<std::string>::create("targetForce", ""))
{
    registerProperty(_myParticlesPerFrame);
    registerProperty(_myPosition);
    registerProperty(_myLastPosition);
    registerProperty(_myPositionSpread);
    registerProperty(_myTargetPositionSpread);
    registerProperty(_myMaxVelocity);
    registerProperty(_myLifetime);
    registerProperty(_myLifetimeSpread);
    registerProperty(_myTargetForceName);

    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(std::string(initvalue01_fp));

    _myInitValueShader = CGShader::create(vfiles, ffiles, "main", "main");
    _myInitValueShader->load();
}

void cclib::GPUTargetEmitter::update(float theDeltaTime)
{
    // udpate shader parameters
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
    
    // emit new particles
    int numberOfParticles = static_cast<int>(_myParticlesPerFrame->getValue<float>());
    
    cclib::Vector3f velocity(0.0f, 0.0f, 0.0f); 
    float maxVelocity = _myMaxVelocity->getValue<float>();

    cclib::Vector3f position = _myPosition->getValue<Vector3f>();
    cclib::Vector3f lastPosition = _myLastPosition->getValue<Vector3f>();
    cclib::Vector3f positionSpread = _myPositionSpread->getValue<Vector3f>();
    
    float lifetime = _myLifetime->getValue<float>(); 
    float lifetimeSpread = _myLifetimeSpread->getValue<float>();
    
    cclib::Vector3f positionRandom;
    cclib::Vector3f velocityRandom;
    cclib::Vector3f colorRandom;
    
    cclib::Color color(1.0f, 1.0f, 1.0f, 1.0f);

    std::string targetForceName = _myTargetForceName->getValue<std::string>(); 

    // XXX fail checks.
    unity_plugin::ParticlesWrapperPtr p =_myParticlesWrapper->getValue<unity_plugin::ParticlesWrapperPtr>();
    GPUTargetForcePtr targetForce = CC_DYN_PTR_CAST<GPUTargetForce>(p->getComponentByName(targetForceName));
    targetForce->getTargetBuffer()->beginDraw();
    _myInitValueShader->start();

    for(int x = 0; x < numberOfParticles; x++) {
        // position
        float positionBlend = (float)x/(float)numberOfParticles;
        
        cclib::Vector3f pos = lastPosition;
        pos.interpolate(positionBlend, position);
        
        // position
        positionRandom.randomize(positionSpread);
        pos = pos + positionRandom; 
        
        // target position
        cclib::Vector3f targetPosition = position;
        positionRandom.randomize(positionSpread);
        targetPosition = targetPosition + positionRandom; 
        
        // velocity
        cclib::Vector3f vel = velocity;
        velocityRandom.randomize(maxVelocity);
        vel = vel + velocityRandom; 
        
        // lifetime
        float lt = lifetime;
        bool permanent = (lifetime == -1);
        if (!permanent) {
            lt += cclib::random<float>(-lifetimeSpread, lifetimeSpread);
        }

        GPUParticlePtr particle = emit(color, pos, vel, lt, permanent);
      
        // get assigned target force
        Graphics::beginShape(GL_POINTS);
        Graphics::textureCoords(0, targetPosition);
        Graphics::textureCoords(1, color.r(), color.g(), color.b(), color.a());
        Graphics::vertex(particle->x() + 0.5f, particle->y() + 0.5f);
        Graphics::endShape();
        
        // TODO: read color from texture based on target position 
    }
    
    _myInitValueShader->end();
    targetForce->getTargetBuffer()->endDraw();
}
