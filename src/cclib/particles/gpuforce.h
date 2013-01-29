#ifndef __CCLIB_GPUFORCE_INCLUDED__
#define __CCLIB_GPUFORCE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tr1/memory>
#include <GL/glew.h>

#include <particles/gpuparticles.h>
#include <particles/gpuupdateshader.h>

namespace cclib {


class GPUForce {

    protected:
        GPUForce(const std::string & theShaderTypeName);
        
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShaderPtr _myVelocityShader;
        GPUParticlesPtr _myParticles;
	
	    float _myStrength; 
        CGparameter _myStrengthParameter;
	
        void setupParameter(int theWidth, int theHeight);
    
    public: 
        typedef std::tr1::shared_ptr<GPUForce> Ptr;
        
        virtual ~GPUForce() {};	
	    void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr  theShader, 
                int theIndex, int theWidth, int theHeight); 
        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader, 
                std::string theIndex, int theWidth, int theHeight);
        void setSize(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void reset();
        void strength(float theStrength);
        CGparameter parameter(const std::string & theName);
};

}; // namespace

#endif // includeguard
