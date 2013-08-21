#ifndef __CCLIB_SEPARATEGAUSSBLUR_INCLUDED__
#define __CCLIB_SEPARATEGAUSSBLUR_INCLUDED__

#include <cclib.h>
#include <vector>
#include <gl/cgshader.h>
#include <gl/convolutionshader.h>

namespace cclib {

class SeparateGaussianBlur : public ConvolutionShader {

    public:
        SeparateGaussianBlur(float theMaximumRadius, int theWidth, int theHeight, int theRes=1); 
        virtual ~SeparateGaussianBlur() {};

        void radius(float theRadius);
        void beginDraw();
        void endFirstPass();
        void endSecondPass();
        void endDraw();
        
        Texture2DPtr blurredTexture();

    private:
        std::vector<float> calculateKernel(float theRadius);

        int _rows;
        int _intRadius;
        std::vector<float> _matrix;
    
        int _width;
        int _height;
        int _res;

        RenderBufferPtr _texture1;
        RenderBufferPtr _texture2;
    

    protected:
        
};

};

#endif

