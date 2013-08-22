
#include "gpubloom.h"
#include <gl/graphics.h>
#include <gl/separategaussianblur.h>
#include <gl/renderbuffer.h>

#include <stringified_shaders/bloom_vert.glsl.h>
#include <stringified_shaders/bloom_frag.glsl.h>

using namespace cclib;

GPUBloom::GPUBloom(int theWidth, int theHeight) :
    _highlightColor(0.0),
    _highlightScale(2.0),
    _highlightPow(1.0),
    _debugBloom(true),
    _applyBloom(true),
    _blurRadius(10),
    _blurRadius2(10)
{
    TextureAttributesPtr myTextureAttributes = cclib::TextureAttributes::create(16, 3);
    FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create(myTextureAttributes, 1);
    
    _myRenderTexture = RenderBuffer::create(myAttributes, theWidth, theHeight);

    _myBlur = SeparateGaussianBlurPtr(new SeparateGaussianBlur(10, theWidth, theHeight, 1));

    std::vector<std::string> vfiles, ffiles;
    vfiles.push_back(std::string(bloom_vert_glsl));
    ffiles.push_back(std::string(bloom_frag_glsl));
    
    _myBloomShader = GLSLShader::create(vfiles, ffiles);
    _width = theWidth;
    _height = theHeight;
}

void 
GPUBloom::start()
{
    if (!_applyBloom) {
        return;
    }
    
    _myRenderTexture->beginDraw();
}

void
GPUBloom::end() {
    if (!_applyBloom) {
        return;
    }
    endCapture();
    applyBloom();
}

void
GPUBloom::endCapture() {
    _myRenderTexture->endDraw();

    _myBlur->radius(_blurRadius);
    Graphics::clearColor(0,0,0,1);
    Graphics::clear();
    Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
    _myBlur->beginDraw();
    Graphics::clear(); 
    Graphics::image(_myRenderTexture->attachment(0), -_width/2, -_height/2, _width, _height);
    _myBlur->endDraw();
    Graphics::clear();
}

void
GPUBloom::applyBloom() {
	if(_debugBloom) {
        Graphics::image(_myRenderTexture->attachment(0), -_width/2, -_height/2, _width/2, _height/2);
        Graphics::blend(BLEND_MODE_ADD);
        
        Graphics::image(_myBlur->blurredTexture(), 0, 0, _width/2, _height/2);
        Graphics::blend(BLEND_MODE_ADD);
        
        return;
    }
		
    _myBloomShader->start();
    _myBloomShader->setUniform1i("texture", 0);
    _myBloomShader->setUniform1f("highlightRange", _highlightColor);
    _myBloomShader->setUniform1f("highlightScale", _highlightScale);
    _myBloomShader->setUniform1f("highlightPow", _highlightPow);
  
    Graphics::image(_myBlur->blurredTexture(), -_width/2, -_height/2, _width, _height);
    _myBloomShader->end();
    
    Graphics::blend();
}

void 
GPUBloom::startBlur() {
    _myRenderTexture->beginDraw();
}

void
GPUBloom::endBlur(bool renderResultsFlag) {
    _myRenderTexture->endDraw();

    _myBlur->radius(_blurRadius2);
    Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
    _myBlur->beginDraw();
    
    Graphics::image(_myRenderTexture->attachment(0), -_width/2, -_height/2, _width, _height);
    _myBlur->endDraw(renderResultsFlag);
}

RenderBufferPtr
GPUBloom::getRenderTexture() {
    return _myRenderTexture;
};

RenderBufferPtr
GPUBloom::getBlurredFBO() {
    return _myBlur->blurredFBO();
};

Texture2DPtr
GPUBloom::getBlurredTexture() {
    return _myBlur->blurredTexture();
};
