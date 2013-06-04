
#ifndef __CCLIB_GPUDISPLAY_SHADER_INCLUDED__
#define __CCLIB_GPUDISPLAY_SHADER_INCLUDED__

#include <cclib.h>
#include <gl/cgshader.h>

#include <stringified_shaders/points/display.fp.h>
#include <stringified_shaders/points/display.vp.h>

namespace cclib {

class GPUDisplayShader : public CGShader {

    private:
        CGparameter _myPointSizeParameter;
        CGparameter _myMinPointSizeParameter;
        CGparameter _myMaxPointSizeParameter;
        CGparameter _myTangHalfFovParameter;
//        CGparameter _myColorTextureParameter;
    
        GPUDisplayShader(const std::vector<std::string> & theVertexFile, const std::vector<std::string> & theFragmentFile);

    public:
        static GPUDisplayShaderPtr create(const std::string & theVertexFile = std::string(display_vp), 
                                          const std::string & theFragmentFile = std::string(display_fp));
        void setPointSize(float thePointSize);
        void setMinPointSize(float theMinPointSize);
        void setMaxPointSize(float theMaxPointSize);
        void setTangHalfFov(float theTangHalfFov);
//        void setColorTexture(int theID);
};
};

#endif

