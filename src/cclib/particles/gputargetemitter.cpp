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

using namespace cclib;

GPUTargetEmitter::GPUTargetEmitter(GPUParticlesPtr theParticles, int theStart, int theNumberParticles) : 
    GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles),
    _myNumberOfParticles(theNumberParticles),
    
    // emitting properties
    _myParticlesPerFrame(Property_<float>::create("particlesPerFrame", 1000.0f)),
    _myPosition(Property_<Vector3f>::create("position", Vector3f(0,0,0))),
    _myLastPosition(Property_<Vector3f>::create("lastPosition", Vector3f(0,0,0))),
    _myPositionSpread(Property_<Vector3f>::create("positionSpread", Vector3f(0,0,0))),
    _myTargetPositionSpread(Property_<Vector3f>::create("targetPositionSpread", Vector3f(0,0,0))),
    _myMaxVelocity(Property_<float>::create("maxVelocity", 0.0f)),
    _myLifetime(Property_<float>::create("lifetime", 10000)),
    _myLifetimeSpread(Property_<float>::create("lifetimeSpread", 100)),
    _myTargetForce(Property_<Component::Ptr>::create("targetForce", Component::Ptr())),
    _myTextureScale(Property_<float>::create("textureScale", 0.0f)),
    _myTextureOffset(Property_<Vector3f>::create("textureOffset", Vector3f(0,0,0))),

    // color texture
    _myTextureId(Property_<float>::create("textureId", 0.0f)),
    _myLastTextureId(0),
    _myTextureWidth(0),
    _myTextureHeight(0),
    _myTextureData()
{
    registerProperty(_myParticlesPerFrame);
    registerProperty(_myPosition);
    registerProperty(_myLastPosition);
    registerProperty(_myPositionSpread);
    registerProperty(_myTargetPositionSpread);
    registerProperty(_myMaxVelocity);
    registerProperty(_myLifetime);
    registerProperty(_myLifetimeSpread);
    registerProperty(_myTargetForce);
    registerProperty(_myTextureId);
    registerProperty(_myTextureScale);
    registerProperty(_myTextureOffset);

    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(std::string(initvalue01_fp));

    _myInitValueShader = CGShader::create(vfiles, ffiles, "main", "main");
    _myInitValueShader->load();
}

float
GPUTargetEmitter::clip(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

Color
GPUTargetEmitter::getPixel(float x, float y, float scale, Vector3f offset) {
    if (_myTextureData.size() == 0) {
        return Color(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    x = (x + offset.x()) * scale;
    y = (y + offset.y()) * scale;

    int cx = (int)clip(x, 0.0f, _myTextureWidth-1.0f);
    int cy = (int)clip(y, 0.0f, _myTextureHeight-1.0f);
    
    int i = cy * _myTextureWidth * 3 + cx * 3;
    
    return Color(_myTextureData[i]/255.0f,
                 _myTextureData[i+1]/255.0f,
                 _myTextureData[i+2]/255.0f, 1.0f);
}

void GPUTargetEmitter::refreshTextureData() {
    unsigned int myCurrentTextureId = static_cast<int>(_myTextureId->getValue<float>());
    // texture did not change
    if (myCurrentTextureId == _myLastTextureId) {
        return;
    }
    
    // texture id not set
    _myLastTextureId = myCurrentTextureId;
    if (myCurrentTextureId == 0) {
        _myTextureHeight = 0;
        _myTextureWidth = 0;
        _myTextureData.clear();
        return;
    }
    
    // update texture information
    glBindTexture( GL_TEXTURE_2D, _myLastTextureId );
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_myTextureWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_myTextureHeight);
    
    unsigned char * pixelData = new unsigned char[_myTextureWidth * _myTextureHeight * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    // assign texture data
    _myTextureData = std::vector<unsigned char>(pixelData, pixelData + (_myTextureWidth * _myTextureHeight * 3) );
    
    // cleanup
    delete pixelData;
    glBindTexture( GL_TEXTURE_2D, 0);
}

void GPUTargetEmitter::update(float theDeltaTime)
{
    refreshTextureData();
    
    // udpate shader parameters
    GPUIndexParticleEmitter::update(theDeltaTime);
    
    // emit new particles
    int numberOfParticles = static_cast<int>(_myParticlesPerFrame->getValue<float>());
    
    Vector3f velocity(0.0f, 0.0f, 0.0f); 
    float maxVelocity = _myMaxVelocity->getValue<float>();

    Vector3f position = _myPosition->getValue<Vector3f>();
    Vector3f lastPosition = _myLastPosition->getValue<Vector3f>();
    Vector3f positionSpread = _myPositionSpread->getValue<Vector3f>();
    Vector3f targetPositionSpread = _myTargetPositionSpread->getValue<Vector3f>();
    
    float lifetime = _myLifetime->getValue<float>(); 
    float lifetimeSpread = _myLifetimeSpread->getValue<float>();
    
    Vector3f positionRandom;
    Vector3f velocityRandom;
    Vector3f colorRandom;

    Component::Ptr targetForceComponent = _myTargetForce->getValue<Component::Ptr>();
    if (!targetForceComponent) {
        // update has not been called, so all the references are not set (yet)
        return;
    }
    
    GPUTargetForcePtr targetForce = CC_DYN_PTR_CAST<GPUTargetForce>(targetForceComponent);
    if (!targetForce) {
        // component has not been assigned correctly
        return;
    }
    
    targetForce->getTargetBuffer()->beginDraw();
    _myInitValueShader->start();
    
    // init random
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    
    std::normal_distribution<float> pX(0.0f, positionSpread.x());
    std::normal_distribution<float> pY(0.0f, positionSpread.y());
    std::normal_distribution<float> pZ(0.0f, positionSpread.z());

    std::normal_distribution<float> tX(0.0f, targetPositionSpread.x());
    std::normal_distribution<float> tY(0.0f, targetPositionSpread.y());
    std::normal_distribution<float> tZ(0.0f, targetPositionSpread.z());

    float scale = _myTextureScale->getValue<float>();
    Vector3f offset = _myTextureOffset->getValue<Vector3f>();
    
    for(int x = 0; x < numberOfParticles; x++) {
        // position
        float positionBlend = (float)x/(float)numberOfParticles;
        
        Vector3f pos = lastPosition;
        pos.interpolate(positionBlend, position);
        
        Vector3f pos2 = pos;
        
        // position
        positionRandom.set(pX(generator), pY(generator), pZ(generator));
        pos = pos + positionRandom;
        
        // target position
        Vector3f targetPosition = pos2;
        positionRandom.set(tX(generator), tY(generator), tZ(generator));
        targetPosition = targetPosition + positionRandom; 
        
        // velocity
        Vector3f vel = velocity;
        velocityRandom.randomize(maxVelocity);
        vel = vel + velocityRandom; 
        
        // lifetime
        float lt = lifetime;
        bool permanent = (lifetime < 0);
        if (!permanent) {
            lt += random<float>(-lifetimeSpread, lifetimeSpread);
        } else {
            lt = -lifetime;
        }

        Color color = getPixel(targetPosition.x(), targetPosition.y(), scale, offset);

        GPUParticlePtr particle = emit(color, pos, vel, lt, permanent);
      
        if (particle) {
            // get assigned target force
            Graphics::beginShape(GL_POINTS);
            Graphics::textureCoords(0, targetPosition);
            Graphics::textureCoords(1, color.r(), color.g(), color.b(), color.a());
            Graphics::vertex(particle->x() + 0.5f, particle->y() + 0.5f);
            Graphics::endShape();
        }
    }
    
    _myInitValueShader->end();
    targetForce->getTargetBuffer()->endDraw();
}
