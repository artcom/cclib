
#ifndef __CCLIB_GPUTARGETFORCE_INCLUDED__
#define __CCLIB_GPUTARGETFORCE_INCLUDED__

#include <cclib.h>
#include <gl/cgshader.h>
#include <particles/gpuforce.h>
#include <particles/gputargetsetup.h>
#include <gl/shaderbuffer.h>

namespace cclib {

class GPUTargetForce : public GPUForce {

    private:
        CGparameter _myTargetPositionTextureParameter;
        CGparameter _myCenterParameter;
        CGparameter _myScaleParameter;
        CGparameter _myLookAheadParameter;
        CGparameter _myMaxForceParameter;
        CGparameter _myNearDistanceParameter;
        CGparameter _myNearMaxForceParameter;

        int _myInitialTargetTextures;
        std::vector<ShaderBufferPtr> _myTargetPositionTextures;
        ShaderBufferPtr _myTargetBuffer;
        Property_<float>::Ptr _myScale;
        Property_<Vector3f>::Ptr _myCenter;
        Property_<float>::Ptr _myLookAhead;
        Property_<float>::Ptr _myMaxForce; 
        Property_<float>::Ptr _myNearDistance; 
        Property_<float>::Ptr _myNearMaxForce;

        CGShaderPtr _myInitValueShader;
    
        int _myWidth;
        int _myHeight;
        int _myCurrentIndex;
    
    public:
        GPUTargetForce(int theTargetTextures);
        static GPUTargetForcePtr create(int theTargetTextures=0);
    
        void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setSize(int theWidth, int theHeight);
        void setTargets(ShaderBufferPtr theTargetTexture, GPUTargetSetupPtr theSetup,
                    int theX, int theY, int theWidth, int theHeight);
        void setTargets(ShaderBufferPtr theTargetTexture, GPUTargetSetupPtr theSetup,
                    GPUIndexParticleEmitterPtr theGroup);
        void addTargetSetup(GPUTargetSetupPtr theSetup);
        void updateSetup(int theIndex, GPUTargetSetupPtr theSetup,
                     int theX, int theY, int theWidth, int theHeight);
        void updateSetup(int theIndex, GPUTargetSetupPtr theSetup, GPUIndexParticleEmitterPtr theParticleGroup);
        void updateSetup(int theIndex, GPUTargetSetupPtr theSetup);
        void addTargetSetup(ShaderBufferPtr theShaderTexture);
        // void addTargetSetup(int theTextureID);
        void changeSetup(int theIndex);
        ShaderBufferPtr getTargetBuffer(); 
};
};

#endif 
