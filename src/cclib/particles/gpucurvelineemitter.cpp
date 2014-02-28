//
//  gpuindexparticlecurveemitter.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpucurvelineemitter.h"

#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "gl/bufferobject.h"
#include "math/math.h"
#include "particles/gpuparticle.h"
#include "particles/gpuparticles.h"
#include "gl/mesh.h"

#define MAX(x, y) ((y > x)?y:x)  

cclib::GPUCurveLineEmitter::GPUCurveLineEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) : 
    cclib::GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles),
    _myNumberOfParticles(theNumberParticles),

    // emitting properties
    _myParticlesPerSecond(Property_<float>::create("particlesPerSecond", 0.0167f)),
    _myVelocity(Property_<cclib::Vector3f>::create("velocity", Vector3f(0,0,0))),
    _myVelocitySpread(Property_<cclib::Vector3f>::create("velocitySpread", Vector3f(0,0,0))),
    _myMinX(Property_<float>::create("minX", -1000.0f)), 
    _myMaxX(Property_<float>::create("maxX", 1000.0f)), 
    _myRadius(Property_<float>::create("radius", 10.0f)), 
    _myLifetime(Property_<float>::create("lifetime", 10000)),
    _myLifetimeSpread(Property_<float>::create("lifetimeSpread", 100)),
    _myCurveLine(Property_<Component::Ptr>::create("curveLine", Component::Ptr()))
{
    registerProperty(_myParticlesPerSecond);
    registerProperty(_myVelocity);
    registerProperty(_myVelocitySpread);
    registerProperty(_myLifetime);
    registerProperty(_myLifetimeSpread);
    registerProperty(_myMinX);
    registerProperty(_myMaxX);
    registerProperty(_myRadius);
    registerProperty(_myCurveLine);
}

void cclib::GPUCurveLineEmitter::update(float theDeltaTime)
{
    // udpate shader parameters
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
   
    if (!_myCurveLine->getValue<Component::Ptr>()) {
        return;
    }

    Component::Ptr myCurveLine = _myCurveLine->getValue<cclib::Component::Ptr>();

    // emit new particles
    int numberOfParticles = MAX(0, static_cast<int>(_myParticlesPerSecond->getValue<float>() * _myNumberOfParticles * theDeltaTime));
    
    cclib::Vector3f velocity = _myVelocity->getValue<Vector3f>();
    cclib::Vector3f velocitySpread = _myVelocitySpread->getValue<Vector3f>();
    float lifetime = _myLifetime->getValue<float>(); 
    float lifetimeSpread = _myLifetimeSpread->getValue<float>();
    
    bool spreadVelocity = !(velocitySpread.equalsZero());
    bool spreadLifetime = (lifetimeSpread != 0.0f);

    cclib::Vector3f velocityRandom;

    float minX = _myMinX->getValue<float>();
    float maxX = _myMaxX->getValue<float>();
    float radius = _myRadius->getValue<float>();
    cclib::Vector3f radiusOffset(0,0,0);

    cclib::Property::Ptr curvePoints = myCurveLine->getPropertyByName("curvePoints");

    float curveMinX = myCurveLine->get<float>("minX");
    float curveMaxX = myCurveLine->get<float>("maxX");
    float outputScale = myCurveLine->get<float>("outputScale");
    std::vector<cclib::Vector2f> points = curvePoints->getValue<std::vector<cclib::Vector2f> >();

    for(int x = 0; x < numberOfParticles; x++) {
        
        // position that we are interested in
        float xPos = cclib::random<float>(minX, maxX);

        // calculate where this position can be found in the curveLine list
		float relativeX = (xPos - curveMinX) / (curveMaxX - curveMinX);
        float index = ((float)points.size()) * relativeX;

        int i = (int) index;
        i = (i<0)?0:i;
        i = (i>=points.size())?points.size()-1:i;

        cclib::Vector3f pos = cclib::Vector3f(xPos, 
                points[i].x() * outputScale, points[i].y() * outputScale);
        
        radiusOffset.randomize(radius);

        // velocity
        cclib::Vector3f vel = velocity;
        if (spreadVelocity) {
            velocityRandom.randomize(velocitySpread);
            vel = vel + velocityRandom; 
        }
        
        cclib::Color col(1.0f, 1.0f, 1.0f, 1.0f);
        
        // lifetime
        float lt = lifetime;
        if (spreadLifetime) {
            lt += cclib::random<float>(-lifetimeSpread, lifetimeSpread); 
        }

        emit(col, pos + radiusOffset, vel, lt, false);
    }
}
