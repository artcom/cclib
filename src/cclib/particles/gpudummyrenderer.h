#pragma once

#include <cclib.h>
#include "particles/gpuparticlerenderer.h"
#include "gl/vbomesh.h"

#include "particles/gpuparticles.h"

namespace cclib {
      
    class GPUDummyRenderer : public GPUParticleRenderer
    {
        public:
            GPUDummyRenderer() {};

            static GPUDummyRendererPtr create() {
                return GPUDummyRendererPtr(new GPUDummyRenderer());
            };

            virtual void setup(GPUParticlesPtr) {};
            virtual void update(float) {};
            virtual void draw() {};

            VBOMeshPtr mesh() {  
                return VBOMeshPtr(); 
            };
            void setPointSize(float) {};
    };
    
}; // namespace cclib

