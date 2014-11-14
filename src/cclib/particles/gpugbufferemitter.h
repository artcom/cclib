#pragma once

#include <cclib.h>
#include <vector>
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"

namespace cclib {
    
    class GPUGBufferEmitter : public cclib::GPUIndexParticleEmitter
    {
        cclib::CGShaderPtr _myEmitShader;
        GPUParticlesPtr _myParticles;

        Property_<float>::Ptr _myColorBuffer;
        Property_<float>::Ptr _myPositionBuffer;
    
        Property_<float>::Ptr _myMaxLifeTime;
        Property_<float>::Ptr _myMinLifeTime;
        Property_<float>::Ptr _myEmitProbability;
        Property_<float>::Ptr _myPositionJitter;
    
        CGparameter _myPositionTextureParameter;
        CGparameter _myInfoTextureParameter;
        CGparameter _myVelocityTextureParameter;
        CGparameter _myColorTextureParameter;
        
        CGparameter _myGeometryTextureParameter;
        CGparameter _myGeometryColorTextureParameter;

        CGparameter _myGBufferSizeParameter;
        CGparameter _myRandomSeedParameter;
        CGparameter _myEmitProbabilityParameter;
        CGparameter _myMaxLifeTimeParameter;
        CGparameter _myMinLifeTimeParameter;
        CGparameter _myPositionJitterParameter;
        CGparameter _myChannelBlendParameter;
           

    public:
        GPUGBufferEmitter(cclib::GPUParticlesPtr theParticles);
        
        static GPUGBufferEmitterPtr create(cclib::GPUParticlesPtr theParticles) {
            return cclib::GPUGBufferEmitterPtr(new cclib::GPUGBufferEmitter(theParticles));
        };
        
        void update(float theDeltaTime);
        void setData();
    };
    
}; // namespace cclib
