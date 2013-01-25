#ifndef __CCLIB_GPUPARTICLES_INCLUDED__
#define __CCLIB_GPUPARTICLES_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tr1/memory>
#include <GL/glew.h>

namespace cclib {

class GPUUpdateShader;
typedef std::tr1::shared_ptr<GPUUpdateShader> GPUUpdateShaderPtr;

class GPUParticles;
typedef std::tr1::shared_ptr<GPUParticles> GPUParticlesPtr;

class GPUParticle;
typedef std::tr1::shared_ptr<GPUParticle> GPUParticlePtr;

class GPUConstraint;
typedef std::tr1::shared_ptr<GPUConstraint> GPUConstraintPtr;

class GPUForce;
typedef std::tr1::shared_ptr<GPUForce> GPUForcePtr;

class ShaderTexture;
typedef std::tr1::shared_ptr<ShaderTexture> ShaderTexturePtr;

class GPUImpulse;
typedef std::tr1::shared_ptr<GPUImpulse> GPUImpulsePtr;

class GPUImpulse;
typedef std::tr1::shared_ptr<GPUImpulse> GPUImpulsePtr;

class Shader;
typedef std::tr1::shared_ptr<Shader> ShaderPtr;

class GPUParticleRenderer;
typedef std::tr1::shared_ptr<GPUParticleRenderer> GPUParticleRendererPtr;

class GPUParticleEmitter;
typedef std::tr1::shared_ptr<GPUParticleEmitter> GPUParticleEmitterPtr;

// class FloatBuffer; // XXX

class GPUParticles {

    protected: 
        std::map<int, Vector3fPtr> _myPositionUpdates;
        std::vector<GPUParticlePtr> _myLifetimeUpdates; 
	    
        std::vector<GPUForcePtr> _myForces;
	    std::vector<GPUConstraintPtr> _myConstraints;
	    std::vector<GPUImpulsePtr> _myImpulses;
	    
	    int _myWidth;
	    int _myHeight;
	    
        GPUUpdateShaderPtr _myUpdateShader;
	    
        ShaderPtr _myInitValue01Shader;
        ShaderPtr _myInitValue0Shader;
	    
        ShaderTexturePtr _myCurrentDataTexture;
        ShaderTexturePtr _myDestinationDataTexture;
	    
	    double _myCurrentTime;
	    
	    // FloatBuffer _myPositionBuffer;
	    // FloatBuffer _myVelocityBuffer;
	
    private:
        std::vector<GPUParticleEmitterPtr> _myEmitter; 
	    GPUParticleRendererPtr _myParticleRender;
	
	    GPUParticles( GPUParticleRendererPtr theRender,
		    std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints, 
		    std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight); 
    
    public:
        GPUParticlesPtr create( GPUParticleRendererPtr theRender,
		    std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints, 
		    std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight); 

        void addEmitter(GPUParticleEmitterPtr theEmitter); 
        ShaderPtr initValueShader();
        double currentTime();
        void reset();
        int width();
        int height();
        int size();
        ShaderTexturePtr dataTexture();
        Vector3fPtr position(GPUParticlePtr theParticle);
        ShaderTexturePtr destinationDataTexture();
        void setPosition(int theIndex, Vector3fPtr thePosition);
        void updateLifecyle(GPUParticlePtr theParticle);
    
    private:
        void updateManualPositionChanges();
        void initializeNewParticles();
        void changeStates();
        void beforeUpdate();
        void cleanUpParticles();
    
    protected:
        void afterUpdate();
    
    public:

        void update(float theDeltaTime);
        void swapDataTextures();
        void draw();
        GPUParticleRendererPtr renderer();
};
    
}; // namespace

#endif // includeguard


