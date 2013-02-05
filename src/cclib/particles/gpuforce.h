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
	
        virtual void setupParameter(int theWidth, int theHeight);
    
    public: 
        typedef std::tr1::shared_ptr<GPUForce> Ptr;
        
        virtual ~GPUForce() {};	
	    virtual void setShader(GPUParticles * theParticles, GPUUpdateShader *  theShader,
                int theIndex, int theWidth, int theHeight); 
        virtual void setShader(GPUParticles * theParticles, GPUUpdateShader * theShader,
                std::string theIndex, int theWidth, int theHeight);
        virtual void setSize(int theWidth, int theHeight);
        virtual void update(float theDeltaTime);
        virtual void reset();
        virtual void strength(float theStrength);
        virtual CGparameter parameter(const std::string & theName);
};

}; // namespace

#endif // includeguard
