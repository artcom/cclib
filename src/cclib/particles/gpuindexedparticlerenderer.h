//
//  gpuindexedparticlerenderer.h
//  Project
//
//  Created by Frank Eickhoff on 18.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#pragma once

#include <cclib.h>
#include "particles/gpuparticlerenderer.h"
#include "gl/glslshader.h"
#include "gl/vbomesh.h"

#include "particles/gpuparticles.h"

namespace cclib {
    
    class GPUIndexedParticleRenderer;
    typedef std::tr1::shared_ptr<GPUIndexedParticleRenderer> GPUIndexedParticleRendererPtr;
    
    class GPUIndexedParticleRenderer : public cclib::GPUParticleRenderer
    {
    protected:
        
        cclib::VBOMeshPtr _myMesh;
        cclib::GPUParticlesPtr _myParticles;
        cclib::GLSLShaderPtr _myShader;
        float _myPointsize;
        
    public:
        GPUIndexedParticleRenderer();
        
        static GPUIndexedParticleRendererPtr create() {
            return cclib::GPUIndexedParticleRendererPtr(new cclib::GPUIndexedParticleRenderer());
        };
        
        virtual void setup(cclib::GPUParticles* theParticles);
        virtual void update(float theDeltaTime);
        virtual void draw();
        
        cclib::VBOMeshPtr mesh() { return _myMesh; };
        virtual void pointSize(float thePointSize) { _myPointsize = thePointSize; };
    };
    
}; // namespace cclib
