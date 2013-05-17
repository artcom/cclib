
#ifndef __GPUPARTICLEPOINTRENDERER_INCLUDED__
#define __GPUPARTICLEPOINTRENDERER_INCLUDED__

#include <cclib.h>
#include <particles/gpuparticlerenderer.h>

namespace cclib {

class GPUParticlePointRenderer : public GPUParticleRenderer {
   
    protected:
        VBOMeshPtr _myMesh;
        GPUParticlesPtr _myParticles;
    
    private:
        GPUDisplayShaderPtr _myDisplayShader;

    public:
        GPUParticlePointRenderer();
    
        static GPUParticlePointRendererPtr create();

        void setup(GPUParticles * theParticles);
        void update(float theDeltaTime);
        void draw();
	
        VBOMeshPtr mesh();
        void setPointSize(float thePointSize);
        void setPointSizeClamp(float theMinPointSize, float theMaxPointSize);
};
}; // namespace
#endif
