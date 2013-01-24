#ifndef __CCLIB_GPUFORCE_INCLUDED__
#define __CCLIB_GPUFORCE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <gl/gpuupdateshader.h>
#include <Exception.h>

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
