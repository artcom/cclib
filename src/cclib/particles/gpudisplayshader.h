
#ifndef __CCLIB_GPUDISPLAY_SHADER_INCLUDED__
#define __CCLIB_GPUDISPLAY_SHADER_INCLUDED__

#include <cclib.h>
#include <gl/shader.h>

#include <stringified_shaders/points/display.fp.h>
#include <stringified_shaders/points/display.vp.h>

namespace cclib {

class GPUDisplayShader : public Shader {

    private:
        CGparameter _myPointSizeParameter;
        CGparameter _myMinPointSizeParameter;
        CGparameter _myMaxPointSizeParameter;
        CGparameter _myTangHalfFovParameter;
	
        GPUDisplayShader(const std::vector<std::string> & theVertexFile, const std::vector<std::string> & theFragmentFile);

    public:
        static GPUDisplayShaderPtr create(const std::string & theVertexFile = std::string(display_vp), 
                                          const std::string & theFragmentFile = std::string(display_fp));
        void pointSize(float thePointSize);
        void minPointSize(float theMinPointSize);
        void maxPointSize(float theMaxPointSize);
        void tangHalfFov(float theTangHalfFov);
};
};

#endif

