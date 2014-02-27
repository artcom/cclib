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
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"

namespace cclib {
    
    class GPUSimpleEmitter : public cclib::GPUIndexParticleEmitter
    {
        cclib::CGShaderPtr _myCurveEmitShader;
        
        Property_<Vector3f>::Ptr _myVelocity;
        Property_<Vector3f>::Ptr _myVelocitySpread;
        Property_<Vector3f>::Ptr _myPosition;
        Property_<Vector3f>::Ptr _myPositionSpread;
        Property_<Vector3f>::Ptr _myColor;
        Property_<Vector3f>::Ptr _myColorSpread;
        Property_<float>::Ptr _myLifetime;
        Property_<float>::Ptr _myLifetimeSpread;
        Property_<float>::Ptr _myParticlesPerSecond;
       
        int _myNumberOfParticles;

    public:
        GPUSimpleEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
        
        static GPUSimpleEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
            return cclib::GPUSimpleEmitterPtr(new cclib::GPUSimpleEmitter(theParticles, theStart, theNumberParticles));
        };
        
        void update(float theDeltaTime);
    };
    
}; // namespace cclib
