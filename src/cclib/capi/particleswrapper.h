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
#include <map>
#include <cclib.h>

// cclib stuff
#include <cclib.h>
#include <base/component.h>

namespace unity_plugin {

    class ParticlesWrapper;
    typedef CC_PTR<ParticlesWrapper> ParticlesWrapperPtr;
    
    class ParticlesWrapper : public std::enable_shared_from_this<ParticlesWrapper> {
        private:
            ParticlesWrapper();

        public:
            static ParticlesWrapperPtr create() {
                return ParticlesWrapperPtr(new ParticlesWrapper());
            }
            virtual ~ParticlesWrapper() {};

            void setup(void* texture);
            void addForce(const std::string & forceType, std::string & identifier, ParticlesWrapperPtr particlesWrapperPtr=ParticlesWrapperPtr());
            void addCombinedForce(const std::string & forceType, std::string & identifier, std::string & force1, std::string & force2, ParticlesWrapperPtr particlesWrapperPtr=ParticlesWrapperPtr()); 
            void addEmitter(const std::string & emitterType, std::string & identifier, ParticlesWrapperPtr particlesWrapperPtr=ParticlesWrapperPtr());
            void addAnimation(const std::string & animationType);
            void updateSimulation(float theDeltaT);
           
            template<typename T>
            inline void setParameter(const std::string & componentName, const std::string & parameterName, T & value) {
                _componentMap[componentName]->set<T>(parameterName, value);
            };
           
            void setVector2fIndexParameter(const std::string & componentName, const std::string & parameterName, 
                    float x, float y, int index) 
            {
                cclib::Vector2f vec(x, y);
                _componentMap[componentName]->get<std::vector<cclib::Vector2f> >(parameterName)[index] = vec; 
            };
            
            void setVector3fIndexParameter(const std::string & componentName, const std::string & parameterName, 
                    float x, float y, float z, int index) 
            {
                cclib::Vector3f vec(x, y, z);
                _componentMap[componentName]->get<std::vector<cclib::Vector3f> >(parameterName)[index] = vec; 
            };

            void setComponentReference(const std::string & componentName, const std::string & parameterName, const std::string & referenceName) {
                _componentMap[componentName]->set<cclib::Component::Ptr>(parameterName, _componentMap[referenceName]);
            };

            void copyResults();
            void setColorTexture(void* texture);
            void setInfoTexture(void* texture);
            cclib::Component::Ptr getComponentByName(std::string componentName);

        private:
            void setDefaultGraphicsState();
            cclib::GPUForcePtr createForceFromString(const std::string & forceType);
            cclib::GPUIndexParticleEmitterPtr createEmitterFromString(const std::string & emitterType, 
                    unsigned int texWidth, unsigned int texHeight); 
            void createEmitters(unsigned int texWidth, unsigned int texHeight);

        private:
            cclib::GPUParticlesPtr _particleSystem;
            std::vector<cclib::GPUForcePtr> _forces;
            std::vector<cclib::GPUIndexParticleEmitterPtr> _emitters;
            std::vector<std::string> _emittersToCreate;

            std::map<std::string, cclib::Component::Ptr> _componentMap;
    
            GLuint _positionTexture;
            GLuint _colorTexture;
            GLuint _infoTexture;
    };
}

#endif /* defined(__cclib_plugin__ParticleRendering__) */

