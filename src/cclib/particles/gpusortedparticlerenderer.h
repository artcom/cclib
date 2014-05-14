
#pragma once

#include <cclib.h>
#include "particles/gpuparticlerenderer.h"
#include "particles/gpuparticlesort.h"
#include "particles/gpuparticles.h"
#include "gl/glslshader.h"
#include "gl/vbomesh.h"
#include "gl/shaderbuffer.h"

namespace cclib {

	class GPUSortedParticleRenderer : public cclib::GPUParticleRenderer
	{
	public:
		typedef CC_PTR<GPUSortedParticleRenderer> GPUSortedParticleRendererPtr;
		
		GPUSortedParticleRenderer();
		
		static GPUSortedParticleRendererPtr create() {
			return GPUSortedParticleRendererPtr(new GPUSortedParticleRenderer());
		};
		
		void setup(cclib::GPUParticlesPtr theParticles);
		void update(float theDeltaTime);
		void draw();
		cclib::VBOMeshPtr mesh(){ return _myMesh; };
		void setPointSize(float thePointSize) { _myPointsize = thePointSize; };
		
		void drawDebug();
		
		cclib::GPUParticleSortPtr sort() { return _mySort; };
		
	private:
		
		
		cclib::GLSLShaderPtr _myShader;
		cclib::VBOMeshPtr _myMesh;
		cclib::GPUParticlesPtr _myParticles;
		
		cclib::GPUParticleSortPtr _mySort;
		cclib::ShaderBufferPtr _myColorBuffer;
		
		float _myPointsize;
	};

}; // namespace cclib
