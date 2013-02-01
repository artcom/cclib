
#ifndef __GPUPARTICLEPOINTRENDERER_INCLUDED__
#define __GPUPARTICLEPOINTRENDERER_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUParticlePointRenderer : public GPUParticleRenderer {
   
    protected:
        VBOMeshPtr _myMesh;

    private:
        GPUParticlesPtr _myParticles;
        GPUDisplayShaderPtr _myDisplayShader;
        GPUParticlePointRenderer();

    public:
        static GPUParticlePointRendererPtr create();

        void setup(GPUParticlesPtr theParticles);
        void update(float theDeltaTime);
        void draw();
	
        VBOMeshPtr mesh();
        void pointSize(float thePointSize);
        void pointSizeClamp(float theMinPointSize, float theMaxPointSize);
};
}; // namespace
#endif
