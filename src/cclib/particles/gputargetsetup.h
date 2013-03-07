#ifndef __CCLIB_GPUTARGETSETUP_INTERFACE_INCLUDED__
#define __CCLIB_GPUTARGETSETUP_INTERFACE_INCLUDED__

#include <cclib.h>

namespace cclib {
class GPUTargetSetup
{
public:
    virtual ~GPUTargetSetup() {};
    virtual void setParticleTargets(int theX, int theY, int theWidth, int theHeight) = 0;
    virtual void setParticleTargets(GPUIndexParticleEmitterPtr theGroup) = 0;
};
}; // namespace

#endif