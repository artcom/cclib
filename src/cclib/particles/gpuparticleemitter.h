#ifndef __CCLIB_GPUPARTICLEEMITTER_INCLUDED__
#define __CCLIB_GPUPARTICLEEMITTER_INCLUDED__

#include <base/property.h>
#include <base/component.h>
#include <cclib.h>

namespace cclib {

class GPUParticleEmitter {
    public:
        virtual void setData() = 0; 
        virtual void update(float theDeltaTime) = 0;
};

}; // namespace 

#endif
