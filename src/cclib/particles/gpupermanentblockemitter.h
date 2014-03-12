
#pragma once

#include <cclib.h>
#include <vector>
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"

namespace cclib {
    
    class GPUPermanentBlockEmitter : public cclib::GPUIndexParticleEmitter
    {
        cclib::CGShaderPtr _myCurveEmitShader;
        
        bool _emittedOnce; 
        float _myWidth;
        float _myHeight;

    public:
        GPUPermanentBlockEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theWidth, int theHeight);
        
        static GPUPermanentBlockEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theWidth, int theHeight) {
            return cclib::GPUPermanentBlockEmitterPtr(new cclib::GPUPermanentBlockEmitter(theParticles, theStart, theWidth, theHeight));
        };
        
        void update(float theDeltaTime);
    };
    
}; // namespace cclib
