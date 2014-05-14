
#ifndef __CCLIB_GPUYFORCEBLEND_INCLUDED__
#define __CCLIB_GPUYFORCEBLEND_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUYForceBlend : public GPUForce {

    private:
        static int MAX_STATES;
    
        Property_<float>::Ptr _myY1;
        Property_<float>::Ptr _myY2;
        Property_<float>::Ptr _myBlendWidth;
    
        CGparameter _myY1Parameter;
        CGparameter _myY2Parameter;
        CGparameter _myBlendWidthParameter;
    
        GPUForcePtr _myForce1;
        GPUForcePtr _myForce2;
    
        GPUYForceBlend();

    public:
        static GPUYForceBlendPtr create();
    
        void initialize(GPUForcePtr theForce1, GPUForcePtr theForce2); 

        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader, int theIndex, int theWidth, int theHeight);
        void setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader, std::string theIndex, int theWidth, int theHeight);
    
	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setSize(int theWidth, int theHeight);
};	
};

#endif 
