//
//  gpuindexparticlecurveemitter.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpusimpleemitter.h"

#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "gl/bufferobject.h"
#include "math/math.h"
#include "particles/gpuparticle.h"
#include "gl/cgshader.h"
#include "gl/shaderbuffer.h"
#include "particles/gpuparticles.h"
#include "gl/mesh.h"

#define MAX(x, y) ((y > x)?y:x)  

cclib::GPUSimpleEmitter::GPUSimpleEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) : 
    cclib::GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles),
    _myNumberOfParticles(theNumberParticles),
    
    // emitting properties
    _myParticlesPerSecond(Property_<float>::create("particlesPerSecond", 0.0167f)),
    _myVelocity(Property_<cclib::Vector3f>::create("velocity", Vector3f(0,0,0))),
    _myVelocitySpread(Property_<cclib::Vector3f>::create("velocitySpread", Vector3f(0,0,0))),
    _myPosition(Property_<cclib::Vector3f>::create("position", Vector3f(0,0,0))),
    _myPositionSpread(Property_<cclib::Vector3f>::create("positionSpread", Vector3f(0,0,0))),
    _myColor(Property_<cclib::Vector3f>::create("color", Vector3f(0,0,0))),
    _myColorSpread(Property_<cclib::Vector3f>::create("colorSpread", Vector3f(0,0,0))),
    _myLifetime(Property_<float>::create("lifetime", 10000)),
    _myLifetimeSpread(Property_<float>::create("lifetimeSpread", 100))
{
    registerProperty(_myParticlesPerSecond);
    registerProperty(_myVelocity);
    registerProperty(_myVelocitySpread);
    registerProperty(_myPosition);
    registerProperty(_myPositionSpread);
    registerProperty(_myColor);
    registerProperty(_myColorSpread);
    registerProperty(_myLifetime);
    registerProperty(_myLifetimeSpread);
}

void cclib::GPUSimpleEmitter::update(float theDeltaTime)
{
    // udpate shader parameters
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
    
    // emit new particles
    int numberOfParticles = MAX(0, static_cast<int>(_myParticlesPerSecond->getValue<float>() * _myNumberOfParticles * theDeltaTime));
    
    cclib::Vector3f velocity = _myVelocity->getValue<Vector3f>();
    cclib::Vector3f velocitySpread = _myVelocitySpread->getValue<Vector3f>();
    cclib::Vector3f position = _myPosition->getValue<Vector3f>();
    cclib::Vector3f positionSpread = _myPositionSpread->getValue<Vector3f>();
    cclib::Vector3f color = _myColor->getValue<Vector3f>();
    cclib::Vector3f colorSpread = _myColorSpread->getValue<Vector3f>();
    float lifetime = _myLifetime->getValue<float>(); 
    float lifetimeSpread = _myLifetimeSpread->getValue<float>();
    
    bool spreadVelocity = !(velocitySpread.equalsZero());
    bool spreadColor    = !(colorSpread.equalsZero());
    bool spreadPosition = !(positionSpread.equalsZero());
    bool spreadLifetime = (lifetimeSpread != 0.0f);

    cclib::Vector3f positionRandom;
    cclib::Vector3f velocityRandom;
    cclib::Vector3f colorRandom;

    for(int x = 0; x < numberOfParticles; x++) {
        // position
        cclib::Vector3f pos = position;
        if (spreadPosition) {
            positionRandom.randomize(positionSpread);
            pos = pos + positionRandom; 
        }
        
        // velocity
        cclib::Vector3f vel = velocity;
        if (spreadVelocity) {
            velocityRandom.randomize(velocitySpread);
            vel = vel + velocityRandom; 
        }
        
        // color
        cclib::Vector3f colv = color;
        if (spreadColor) {
            colorRandom.randomize(colorSpread);
            colv = colv + colorRandom; 
        }
        cclib::Color col(colv.x(), colv.y(), colv.z(), 1.0f);
        
        // lifetime
        float lt = lifetime;
        if (spreadLifetime) {
            lt += cclib::random<float>(-lifetimeSpread, lifetimeSpread); 
        }

        emit(col, pos, vel, lt, false);
    }
}
