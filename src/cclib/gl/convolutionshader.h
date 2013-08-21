#ifndef __CCLIB_CONVOLUTIONSSHADER_INCLUDED__
#define __CCLIB_CONVOLUTIONSSHADER_INCLUDED__

#include <cclib.h>
#include <vector>
#include <gl/cgshader.h>

namespace cclib {

class ConvolutionShader : public CGShader {

    public:
        ConvolutionShader();
        // ConvolutionShader(const std::string & theShader);
        // ConvolutionShader(const int & theKernelWidth, const int & theKernelHeight);
        // ConvolutionShader(const std::vector<float> & theKernel, const int & theKernelWidth, const int & theKernelHeight);

        virtual ~ConvolutionShader() {};

        void setKernel(const std::vector<float> & theKernel, const int & theKernelWidth, const int & theKernelHeight);
        void texture(Texture2DPtr theTexture);
        void flipKernel();
        void updateOffsets();

    private:
        float _pixelWidth;
        float _pixelHeight;
        CGparameter _kernelValueParameter;
        CGparameter _offsetParameter;

    protected:
        int _kernelWidth;
        int _kernelHeight;
        int _kernelSize;

        void initKernel(const std::vector<float> & theKernel);
        void updateKernel(const std::vector<float> & theKernel);
};

};

#endif
