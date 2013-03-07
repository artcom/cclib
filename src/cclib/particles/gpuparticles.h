#ifndef __CCLIB_GPUPARTICLES_INCLUDED__
#define __CCLIB_GPUPARTICLES_INCLUDED__

#include <cclib.h>

namespace cclib {

// class FloatBuffer; // XXX

class GPUParticle;
typedef std::tr1::shared_ptr<GPUParticle> GPUParticlePtr;

class GPUParticles;
typedef std::tr1::shared_ptr<GPUParticles> GPUParticlesPtr;

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
	    
        ShaderBufferPtr _myCurrentDataTexture;
        ShaderBufferPtr _myDestinationDataTexture;
	    
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
        static GPUParticlesPtr create( GPUParticleRendererPtr theRender,
		    std::vector<GPUForcePtr> & theForces, std::vector<GPUConstraintPtr> & theConstraints, 
		    std::vector<GPUImpulsePtr> & theImpulse, int theWidth, int theHeight); 

        void addEmitter(GPUParticleEmitterPtr theEmitter); 
        ShaderPtr initValueShader();
        double currentTime();
        void reset();
        int width();
        int height();
        int size();
        ShaderBufferPtr dataTexture();
        Vector3fPtr position(GPUParticlePtr theParticle);
        ShaderBufferPtr destinationDataTexture();
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


