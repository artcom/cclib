//
//  gpuindexparticlecurveemitter.h
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#pragma once

#include <cclib.h>
#include <vector>
#include "particles/gpuindexparticleemitter.h"
#include "gl/shader.h"
#include "particles/gpunoise.h"

namespace cclib {
    
    class GPUIndexParticleCurveEmitter;
    typedef std::tr1::shared_ptr<GPUIndexParticleCurveEmitter> GPUIndexParticleCurveEmitterPtr;
    
    class GPUIndexParticleCurveEmitter : public cclib::GPUIndexParticleEmitter
    {
        cclib::ShaderPtr _myCurveEmitShader;
        CGparameter _myRadiusParameter;
        CGparameter _myOffsetParameter;
        CGparameter _myScaleParameter;
        CGparameter _myOutputScaleParameter;
        
        float _myOffset;
        float _myScale;
        float _myOutputScale;
        float _myRadius;
        float _mySpeed;
        
    public:
//        GPUIndexParticleCurveEmitter();
        GPUIndexParticleCurveEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
        
//        static GPUIndexParticleCurveEmitterPtr create() {
//            return cclib::GPUIndexParticleCurveEmitterPtr(new cclib::GPUIndexParticleCurveEmitter());
//        };
        
        static GPUIndexParticleCurveEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
            return cclib::GPUIndexParticleCurveEmitterPtr(new cclib::GPUIndexParticleCurveEmitter(theParticles, theStart, theNumberParticles));
        };
        
        void update(float theDeltaTime);
        
        void fillPositionData(cclib::BufferPtr theBuffer);
//        void fillPositionData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticle> theParticles);
//        void transferEmitData();
        void transferData();
        
        void setScale(float value) { _myScale = value; };
        void setOutputScale(float value) { _myOutputScale = value; };
        void setRadius(float value) { _myRadius = value; };
        void setSpeed(float value) { _mySpeed = value; };
        void setOffset(float value) { _myOffset = value; };
    };
    
}; // namespace cclib
