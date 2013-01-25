#ifndef __CCLIB_GPUPARTICLES_INCLUDED__
#define __CCLIB_GPUPARTICLES_INCLUDED__

#include <cclib.h>

namespace cclib {


class GPUParticles {

    protected: 
        std::map<int, Vector3f> _myPositionUpdates;
        std::vector<GPUParticle::Ptr> _myLifetimeUpdates; 
	    
	    List<GPUForce::Ptr> _myForces;
	    List<GPUConstraint::Ptr> _myConstraints;
	    List<GPUImpulse::Ptr> _myImpulses;
	    
	    int _myWidth;
	    int _myHeight;
	    
        GPUUpdateShader::Ptr _myUpdateShader;
	    
        Shader::Ptr _myInitValue01Shader;
        Shader::Ptr _myInitValue0Shader;
	    
        ShaderTexture::Ptr _myCurrentDataTexture;
        ShaderTexture::Ptr _myDestinationDataTexture;
	    
	    double _myCurrentTime;
	    
	    FloatBuffer _myPositionBuffer;
	    FloatBuffer _myVelocityBuffer;
	
    private:
        std::vector<GPUParticleEmitter::Ptr> _myEmitter; 
	    GPUParticleRenderer::Ptr _myParticleRender;
	
	    GPUParticles( GPUParticleRenderer::Ptr theRender,
		    std::vector<GPUForce::Ptr> & theForces, std::vector<GPUConstraint::Ptr> & theConstraints, 
		    std::vector<GPUImpulse::Ptr> % theImpulse, int theWidth, int theHeight); 
    
    public:
        GPUParticles::Ptr create( GPUParticleRenderer::Ptr theRender,
		    std::vector<GPUForce::Ptr> & theForces, std::vector<GPUConstraint::Ptr> & theConstraints, 
		    std::vector<GPUImpulse::Ptr> & theImpulse, int theWidth, int theHeight); 

        void addEmitter(GPUParticleEmitter::Ptr theEmitter); 
        Shader::Ptr initValueShader();
        double currentTime();
        void reset();
        int width();
        int height();
        int size();
        ShaderTexture::Ptr dataTexture();
        Vector3f::Ptr position(GPUParticle::Ptr theParticle);
        ShaderTexture::Ptr destinationDataTexture();
        void setPosition(int theIndex, Vector3f::Ptr thePosition);
        void updateLifecyle(GPUParticle::Ptr theParticle);
    
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
        GPUParticleRenderer::Ptr renderer();
}
    
}; // namespace

#endif // includeguard


