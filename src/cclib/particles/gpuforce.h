#ifndef __CCLIB_GPUFORCE_INCLUDED__
#define __CCLIB_GPUFORCE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

#include <base/property.h>
#include <base/component.h>

#include <particles/gpuparticles.h>
#include <particles/gpuupdateshader.h>

namespace cclib {

    class GPUForce : public Component
{
    protected:
        GPUForce(const std::string & theShaderTypeName);
        
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShaderPtr _myVelocityShader;
        GPUParticlesPtr _myParticles;
	
        Property_<float>::Ptr _myStrength; 
        CGparameter _myStrengthParameter;
	
    public: 
        typedef CC_PTR<GPUForce> Ptr;
        
        virtual ~GPUForce() {
        };
	   
        virtual void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                int theIndex, int theWidth, int theHeight); 
        virtual void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                std::string theIndex, int theWidth, int theHeight);
        virtual void setSize(int theWidth, int theHeight);
        virtual void update(float theDeltaTime);
        virtual void reset();
        virtual float getStrength();
        virtual void setStrength(float theStrength);
        virtual CGparameter parameter(const std::string & theName);
        virtual void setupParameter(int theWidth, int theHeight);
};

}; // namespace

#endif // includeguard
