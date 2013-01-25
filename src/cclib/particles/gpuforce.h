#ifndef __CCLIB_GPUFORCE_INCLUDED__
#define __CCLIB_GPUFORCE_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUForce {

    protected:
        GPUForce(const std::string & theShaderTypeName);
        
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShader::Ptr _myVelocityShader;
        GPUParticles::Ptr _myParticles;
	
	    float _myStrength; //  = 1;
        CGparameter _myStrengthParameter;
	
        void setupParameter(int theWidth, int theHeight);
    
    public: 
        typedef std::tr1::shared_ptr<GPUForce> Ptr;
        
        virtual ~GPUForce() {};	
	    void setShader(GPUParticles::Ptr theParticles, GPUUpdateShader::Ptr  theShader, 
                int theIndex, int theWidth, int theHeight); 
        void setShader(GPUParticles::Ptr theParticles, GPUUpdateShader::Ptr theShader, 
                std::string theIndex, int theWidth, int theHeight);
        void setSize(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void reset();
        void strength(float theStrength);
        CGparameter parameter(const std::string & theName);
};

}; // namespace

#endif // includeguard
