#include "separategaussianblur.h"
#include <math.h>
#include <gl/texture.h>
#include <gl/framebufferobject.h>
#include <gl/renderbuffer.h>
#include <gl/graphics.h>

using namespace cclib;

SeparateGaussianBlur::SeparateGaussianBlur(float theMaximumRadius, int theWidth, int theHeight, int theRes) 
    : ConvolutionShader(), _res(theRes),
    _texture1(), _texture2()
{   
    _width = theWidth/_res;
    _height = theHeight/_res;
    _intRadius = static_cast<int>( ceil(theMaximumRadius) );
    _rows = _intRadius * 2 + 1;
    _matrix = std::vector<float>(_rows, 0.0);
    
    setKernel(calculateKernel(theMaximumRadius), _rows, 1);
}

void 
SeparateGaussianBlur::radius(float theRadius) {
	updateKernel(calculateKernel(theRadius));
}

void 
SeparateGaussianBlur::beginDraw() {
    if(!_texture1) {
        int numberOfBits = 32;
        int numberOfChannels = 4;

        cclib::TextureAttributesPtr myTextureAttributes = cclib::TextureAttributes::create(numberOfBits, numberOfChannels);
        cclib::FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create(myTextureAttributes, 1);
        // myAttributes->samples(8);
        
        _texture1 = RenderBuffer::create(myAttributes, _width, _height);
        _texture2 = RenderBuffer::create(myAttributes, _width, _height);
        texture(_texture1->attachment(0));
    }

    _texture1->beginDraw();
    Graphics::clear();
    glPushMatrix();
    // Graphics::scale(1.0f/_res);
    glScalef(1.0f/_res, 1.0f/_res, 1.0f/_res);
}

void 
SeparateGaussianBlur::endFirstPass() {
    glPopMatrix();
    _texture1->endDraw();

    _texture2->beginDraw();
    Graphics::clear();
    start();

    Graphics::image(_texture1->attachment(0), -_width/2, -_height/2, _width, _height);
    end();
    flipKernel();
    _texture2->endDraw();

}

void 
SeparateGaussianBlur::endSecondPass() {
    _texture1->beginDraw();
    Graphics::clear();
    start();
    Graphics::image(_texture2->attachment(0), -_width/2, -_height/2, _width, _height);
    end();
    flipKernel();
    _texture1->endDraw();
}


void 
SeparateGaussianBlur::endDraw() {
    endFirstPass();
    endSecondPass();
    Graphics::image(_texture1->attachment(0), - _width/2 * _res, -_height/2 * _res, _width * _res, _height * _res);
}

Texture2DPtr 
SeparateGaussianBlur::blurredTexture() {
    return _texture1->attachment(0);
}

std::vector<float> 
SeparateGaussianBlur::calculateKernel(float theRadius) {		
    float sigma = theRadius/3;
    float sigma22 = 2.0 * sigma * sigma;
    float sigmaPi2 = M_PI * 2.0 * sigma;
    float sqrtSigmaPi2 = static_cast<float>( sqrt(sigmaPi2) );

    float radius2 = theRadius * theRadius;
    float total = 0;
    int index = 0;

    for (int row = -_intRadius; row <= _intRadius; row++) {
        float distance = row * row;

        if (distance > radius2) {
            _matrix[index] = 0;
        } else {
            _matrix[index] = static_cast<float>( exp(-(distance)/sigma22) / sqrtSigmaPi2 );
        }
        
        total += _matrix[index];
        index++;
    }

    std::vector<float> myKernel;

    for (int i = 0; i < _rows; i++) {
        myKernel.push_back(_matrix[i] / total);
    }
    
    return myKernel;
}

