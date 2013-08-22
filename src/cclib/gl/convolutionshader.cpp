
#include "convolutionshader.h"
#include <stringified_shaders/imaging/convolution.fp.h>
#include <gl/texture2d.h>
#include <gl/graphics.h>

using namespace cclib;

ConvolutionShader::ConvolutionShader() :
    CGShader(std::vector<std::string>(), std::vector<std::string>(1, convolution_fp)),
    _pixelWidth(1), _pixelHeight(1),
    _kernelValueParameter(), _offsetParameter(),
    _kernelWidth(1), _kernelHeight(1), _kernelSize(1)
{

}

void 
ConvolutionShader::setKernel(const std::vector<float> & theKernel, const int & theKernelWidth, const int & theKernelHeight) {
    _kernelWidth = theKernelWidth;
    _kernelHeight = theKernelHeight;
    _kernelSize = _kernelWidth * _kernelHeight;

    initKernel(theKernel);
}

void 
ConvolutionShader::texture(Texture2DPtr theTexture) {
    _pixelWidth = 1.0f / theTexture->width();
    _pixelHeight = 1.0f / theTexture->height();

    updateOffsets();
}

void 
ConvolutionShader::flipKernel() {
    int temp = _kernelWidth;
    _kernelWidth = _kernelHeight;
    _kernelHeight = temp;
    updateOffsets();
}

void 
ConvolutionShader::updateOffsets() {
    std::vector<cclib::Vector2f> offsets;
    int xStart = -_kernelWidth / 2;
    int yStart = -_kernelHeight / 2;

    for(int x = 0; x < _kernelWidth;x++) {
        for(int y = 0; y < _kernelHeight; y++) {
            offsets.push_back(Vector2f((xStart + x) * _pixelWidth, (yStart + y) * _pixelHeight));
        }
    }

    parameter2(_offsetParameter, offsets);
    Graphics::checkError();
}

void 
ConvolutionShader::initKernel(const std::vector<float> & theKernel) {
    if(_kernelSize != theKernel.size()) {
        throw new cclib::Exception("The given Kernel of the size XX does not match the given width and height.");
    }
    _kernelValueParameter = fragmentParameter("kernelValue");
    _offsetParameter = fragmentParameter("offset");
    cgSetArraySize(_kernelValueParameter, theKernel.size());
    cgSetArraySize(_offsetParameter, theKernel.size());

    Graphics::checkError();

    load();

    Graphics::checkError();
    parameter1(_kernelValueParameter, theKernel);

}

void 
ConvolutionShader::updateKernel(const std::vector<float> & theKernel) {
    if(_kernelSize != theKernel.size()) {
        throw new cclib::Exception("The given Kernel of the size XX does not match the given width and height.");
    }

    parameter1(_kernelValueParameter, theKernel);
}
