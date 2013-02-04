
#ifndef __CCLIB_GPUPARTICLERENDERER_INCLUDED__
#define __CCLIB_GPUPARTICLERENDERER_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUParticleRenderer {
    public:
        GPUParticleRenderer() {};
	    virtual void setup(GPUParticles * theParticles) = 0;
        virtual void update(float theDeltaTime) = 0;
        virtual void draw() = 0;
        virtual VBOMeshPtr mesh() = 0;
};

};

#endif

