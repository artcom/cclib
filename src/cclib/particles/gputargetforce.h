
#ifndef __CCLIB_GPUATTRACTOR_INCLUDED__
#define __CCLIB_GPUATTRACTOR_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>
#include <particles/gputargetsetup.h>

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
        ShaderPtr _myInitValueShader;
        std::vector<ShaderBufferPtr> _myTargetPositionTextures;
        Property_<float>::Ptr _myScale;
    
        int _myWidth;
        int _myHeight;
        int _myCurrentIndex;
    
    public:
        GPUTargetForce(int theTargetTextures);
        static GPUTargetForcePtr create(int theTargetTextures);
    
        void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void setScale(float theScale);
        float getScale();
        void setLookAhead(float theLookAhead);
        void setMaxForce(float theMaxForce);
        void setCenter(Vector3f & theCenter);
        void setCenter(float theX, float theY, float theZ);
        void setNearDistance(float theNearMaxDistance);
        void setNearMaxForce(float theNearMaxForce);
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

};
};

#endif 
