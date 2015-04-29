//
//  gpuindexparticlecurveemitter.h
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#pragma once

#include <cclib.h>
#include <vector>
#include <random>
#include <cmath>
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"
#include "gl/shaderbuffer.h"
#include "gl/cgshader.h"

namespace cclib {
    
    class GPUTargetEmitter : public cclib::GPUIndexParticleEmitter
    {
        Property_<float>::Ptr _myParticlesPerFrame;
        Property_<Vector3f>::Ptr _myPosition;
        Property_<Vector3f>::Ptr _myLastPosition;
        Property_<Vector3f>::Ptr _myPositionSpread;
        
        Property_<float>::Ptr _myMaxVelocity;
        Property_<Vector3f>::Ptr _myTargetPosition;
        Property_<Vector3f>::Ptr _myTargetPositionSpread;
        
        Property_<float>::Ptr _myLifetime;
        Property_<float>::Ptr _myLifetimeSpread;
        Property_<cclib::Component::Ptr>::Ptr _myTargetForce;
        Property_<float>::Ptr _myTextureId;

        Property_<float>::Ptr _myTextureScale;
        Property_<Vector3f>::Ptr _myTextureOffset;

        int _myNumberOfParticles;
        CGShaderPtr _myInitValueShader;
        
        GLuint _myLastTextureId;
        std::vector<unsigned char> _myTextureData;
        int _myTextureWidth;
        int _myTextureHeight;
        
    private:
        void refreshTextureData();
        Color getPixel(float x, float y, float scale, Vector3f offset);
        float clip(float n, float lower, float upper);
        
    public:
        GPUTargetEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
        
        static GPUTargetEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
            return cclib::GPUTargetEmitterPtr(new cclib::GPUTargetEmitter(theParticles, theStart, theNumberParticles));
        };
        
        void update(float theDeltaTime);
    };
    
}; // namespace cclib
