
#pragma once

#include <cclib.h>
#include <vector>
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"

namespace cclib {
    
    class GPUCurveLineEmitter : public cclib::GPUIndexParticleEmitter
    { 
        Property_<Vector3f>::Ptr _myVelocity;
        Property_<Vector3f>::Ptr _myVelocitySpread;
        Property_<float>::Ptr _myLifetime;
        Property_<float>::Ptr _myLifetimeSpread;
        Property_<float>::Ptr _myParticlesPerSecond;
        
        Property_<float>::Ptr _myMinX;
        Property_<float>::Ptr _myMaxX;
        Property_<float>::Ptr _myRadius;
    
        Property_<Component::Ptr>::Ptr _myCurveLine;
        int _myNumberOfParticles;

    public:
        GPUCurveLineEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
        
        static GPUCurveLineEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
            return cclib::GPUCurveLineEmitterPtr(new cclib::GPUCurveLineEmitter(theParticles, theStart, theNumberParticles));
        };
        
        void update(float theDeltaTime);
    };
    
}; // namespace cclib
