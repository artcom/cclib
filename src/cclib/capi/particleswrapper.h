//
//  ParticleRendering.h
//  cclib_plugin
//
//  Created by Sebastian Hein on 1/14/14.
//  Copyright (c) 2014 artcom. All rights reserved.
//

#ifndef __cclib_plugin__ParticlesWrapper__
#define __cclib_plugin__ParticlesWrapper__

#include <memory>
#include <iostream>
#include <vector>
#include <tr1/memory>
#include <cclib.h>

namespace unity_plugin {

    class ParticlesWrapper;
    typedef std::tr1::shared_ptr<ParticlesWrapper> ParticlesWrapperPtr;
    
    class ParticlesWrapper {
        private:
            ParticlesWrapper();

        public:
            static ParticlesWrapperPtr create() {
                return ParticlesWrapperPtr(new ParticlesWrapper());
            }
            virtual ~ParticlesWrapper() {};

            void setup(void* texturePointer);
            void addForce(const std::string & forceType, std::string & identifier);
            void addAnimation(const std::string & animationType);
            void addEmitter();
            void updateSimulation();
            void setParameter(const std::string & parameterName, float value);
            void copyResults();

        private:
            void setDefaultGraphicsState();

        private:
            cclib::GPUParticlesPtr _particleSystem;
            std::vector<cclib::GPUForcePtr> _forces;
    
            // dummy for now
            cclib::GPUIndexParticleEmitterPtr _myEmitter;
            GLuint _gltex;
    };
}

#endif /* defined(__cclib_plugin__ParticleRendering__) */

