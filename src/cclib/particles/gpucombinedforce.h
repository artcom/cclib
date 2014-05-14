
#ifndef __CCLIB_GPUCOMBINDEDFORCE_INCLUDED__
#define __CCLIB_GPUCOMBINDEDFORCE_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUCombinedForce : public GPUForce {

    private:
        std::vector<GPUForcePtr> _myForces;

        GPUCombinedForce(std::vector<GPUForcePtr> & theForces);
    
    public:
        static GPUCombinedForcePtr create(std::vector<GPUForcePtr> & theForces);
    
        void setupParameter(int theWidth, int theHeight);
    
        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                                int theIndex, int theWidth, int theHeight);
    
        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                   std::string theIndex, int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setSize(int theWidth, int theHeight);
};	
};

#endif 
