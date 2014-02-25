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
    
    class GPUIndexParticleCurveEmitter : public cclib::GPUIndexParticleEmitter
    {
        cclib::CGShaderPtr _myCurveEmitShader;
        CGparameter _myRadiusParameter;
        CGparameter _myOffsetParameter;
        CGparameter _myScaleParameter;
        CGparameter _myOutputScaleParameter;
        
        Property_<float>::Ptr _myOffset;
        Property_<float>::Ptr _myScale;
        Property_<float>::Ptr _myOutputScale;
        Property_<float>::Ptr _myRadius;
        Property_<float>::Ptr _mySpeed;
        
        Property_<Vector3f>::Ptr _myVelocity;
        Property_<Vector3f>::Ptr _myVelocitySpread;
        Property_<Vector3f>::Ptr _myPosition;
        Property_<Vector3f>::Ptr _myPositionSpread;
        Property_<Vector3f>::Ptr _myColor;
        Property_<Vector3f>::Ptr _myColorSpread;
        Property_<float>::Ptr _myLifetime;
        Property_<float>::Ptr _myLifetimeSpread;
        Property_<float>::Ptr _myParticlesPerSecond;
        
    public:
        GPUIndexParticleCurveEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
        
        static GPUIndexParticleCurveEmitterPtr create(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
            return cclib::GPUIndexParticleCurveEmitterPtr(new cclib::GPUIndexParticleCurveEmitter(theParticles, theStart, theNumberParticles));
        };
        
        void update(float theDeltaTime);
        
        virtual void fillPositionData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles);
        virtual void transferEmitData();
        
        void setScale(float value) { _myScale->setValue(value); };
        void setOutputScale(float value) { _myOutputScale->setValue(value); };
        void setRadius(float value) { _myRadius->setValue(value); };
        void setSpeed(float value) { _mySpeed->setValue(value); };
        void setOffset(float value) { _myOffset->setValue(value); };
    };
    
}; // namespace cclib
