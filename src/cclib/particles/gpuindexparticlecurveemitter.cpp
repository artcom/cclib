//
//  gpuindexparticlecurveemitter.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuindexparticlecurveemitter.h"

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

cclib::GPUIndexParticleCurveEmitter::GPUIndexParticleCurveEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) : 
    cclib::GPUIndexParticleEmitter(theParticles, theStart,theNumberParticles),
    _myOffset(Property_<float>::create("offset", 1)),
    _myScale(Property_<float>::create("scale", 0.001)),
    _myOutputScale(Property_<float>::create("outputScale", 1)),
    _myRadius(Property_<float>::create("radius", 129)),
    _mySpeed(Property_<float>::create("speed", 0.09)),
    
    // emitting properties
    _myParticlesPerSecond(Property_<float>::create("particlesPerSecond", 1800)),
    _myVelocity(Property_<cclib::Vector3f>::create("velocity", Vector3f(0,0,0))),
    _myVelocitySpread(Property_<cclib::Vector3f>::create("velocitySpread", Vector3f(0,0,0))),
    _myPosition(Property_<cclib::Vector3f>::create("position", Vector3f(0,0,0))),
    _myPositionSpread(Property_<cclib::Vector3f>::create("positionSpread", Vector3f(0,0,0))),
    _myColor(Property_<cclib::Vector3f>::create("color", Vector3f(0,0,0))),
    _myColorSpread(Property_<cclib::Vector3f>::create("colorSpread", Vector3f(0,0,0))),
    _myLifetime(Property_<float>::create("lifetime", 10000)),
    _myLifetimeSpread(Property_<float>::create("lifetimeSpread", 100))
{
    registerProperty(_myOffset);
    registerProperty(_myScale);
    registerProperty(_myOutputScale);
    registerProperty(_myRadius);
    registerProperty(_mySpeed);
    
    registerProperty(_myParticlesPerSecond);
    registerProperty(_myVelocity);
    registerProperty(_myVelocitySpread);
    registerProperty(_myPosition);
    registerProperty(_myPositionSpread);
    registerProperty(_myColor);
    registerProperty(_myColorSpread);
    registerProperty(_myLifetime);
    registerProperty(_myLifetimeSpread);

    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(std::string(simplex_fp));
    ffiles.push_back(std::string(curvefield_emit_fp));
    
    _myCurveEmitShader = cclib::CGShader::create(vfiles, ffiles);

    _myRadiusParameter = _myCurveEmitShader->fragmentParameter("radius");
    _myOffsetParameter = _myCurveEmitShader->fragmentParameter("offset");
    _myScaleParameter = _myCurveEmitShader->fragmentParameter("scale");
    _myOutputScaleParameter = _myCurveEmitShader->fragmentParameter("outputScale");
    _myCurveEmitShader->load();

    cclib::GPUNoise::attachFragmentNoise((cclib::CGShader*)_myCurveEmitShader.get());
}

void cclib::GPUIndexParticleCurveEmitter::update(float theDeltaTime)
{
    // udpate shader parameters
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
    
    _myOffset->setValue( _myOffset->getValue<float>() + theDeltaTime * _mySpeed->getValue<float>());
    
    _myCurveEmitShader->parameter(_myOffsetParameter,  _myOffset->getValue<float>());
    _myCurveEmitShader->parameter(_myOutputScaleParameter, _myOutputScale->getValue<float>());
    _myCurveEmitShader->parameter(_myScaleParameter, _myScale->getValue<float>());
    _myCurveEmitShader->parameter(_myRadiusParameter, _myRadius->getValue<float>());
    
    // emit new particles
    int numberOfParticles = MAX(0, static_cast<int>(_myParticlesPerSecond->getValue<float>() * theDeltaTime));
    
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
            position.randomize(positionSpread);
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

void cclib::GPUIndexParticleCurveEmitter::fillPositionData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles)
{
    for (unsigned int i=0; i<theParticles.size(); i++)
    {
        cclib::GPUParticlePtr myParticle = theParticles[i];
        
        theBuffer->data()[i * 3 + 0] = myParticle->position()->x();
        theBuffer->data()[i * 3 + 1] = cclib::random<float>();
        theBuffer->data()[i * 3 + 2] = cclib::random<float>(2.0f * M_PI);
    }
}

void cclib::GPUIndexParticleCurveEmitter::transferEmitData()
{
    _myParticles->dataBuffer()->beginDraw();
    _myCurveEmitShader->start();
    
    _myEmitMesh->draw();
    
    _myCurveEmitShader->end();
    _myParticles->dataBuffer()->endDraw();
}
