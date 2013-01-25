#ifndef __CCLIB_GPUIMPULSE_INCLUDED__
#define __CCLIB_GPUIMPULSE_INCLUDED__

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

class GPUImpulse {

    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShader _myVelocityShader;
	
    private:
        float _myStrength;
        CGparameter _myStrengthParameter;
        bool _myTrigger = false;
    
    public:	
        GPUImpulse(const std::string &theShaderTypeName, float theStrength);
        void setShader(GPUUpdateShader::Ptr theShader, int theIndex, int theWidth, int theHeight);
        virtual void setupParameter(int theWidth, int theHeight);

        CGparameter parameter(const std::string & theName);
        void strength(float theStrength);
        void trigger();
        bool isTriggering();
        void update(float theDeltaTime);
}

}; // namespace

#endif // includeguard

