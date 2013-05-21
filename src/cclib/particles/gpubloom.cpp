//
//  gpubloom.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpubloom.h"
#include "gl/graphics.h"

#include "stringified_shaders/bloom_vert.glsl.h"
#include "stringified_shaders/bloom_frag.glsl.h"

cclib::GPUBloom::GPUBloom(int theWidth, int theHeight)
: _cHighlightColor(0), _cHighlightScale(2.266667), _cHighlightPow(0)
{
//    printf("%s\n\t%d x %d\n",__PRETTY_FUNCTION__,theWidth,theHeight);
    
//    cclib::FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create();
//    myAttributes.samples(8);
    
    int theNumberOfTextures = 1;
    int theNumberOfBits = 32, theNumberOfChannels = 4;
    
    cclib::TextureAttributesPtr myTextureAttributes = cclib::TextureAttributes::create(theNumberOfBits, theNumberOfChannels);
    
    cclib::FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create(myTextureAttributes,theNumberOfTextures);
    
    myAttributes->numberOfSamples = 8;
    myAttributes->numberOfBits = theNumberOfBits;
    myAttributes->numberOfChannels = theNumberOfChannels;
    myAttributes->depthBuffer = true;
    
    
//    printf("\tnumberOfSamples %d\n\tnumberOfBits %d\n\tnumberOfChannels %d\n",
//           myAttributes->numberOfSamples,myAttributes->numberOfBits,myAttributes->numberOfChannels
//           );
    
//    _myRenderTexture = cclib::RenderBuffer::create(myAttributes, theWidth, theHeight);
//    _myRenderTexture1 = cclib::RenderBuffer::create(myAttributes, theWidth, theHeight);
    _myRenderTexture = cclib::FrameBufferObject::create(GL_TEXTURE_RECTANGLE_EXT, myAttributes, theWidth, theHeight);
    _myRenderTexture1 = cclib::FrameBufferObject::create(GL_TEXTURE_RECTANGLE_EXT, myAttributes, theWidth, theHeight);
    
//    _myBlur = cclib::GPUSeperateGaussianBlur::create(10, theWidth, theHeight, 1);
    
//    g = theApp.g;


    std::vector<std::string> vfiles, ffiles;
    
    vfiles.push_back(std::string(bloom_vert_glsl));
    ffiles.push_back(std::string(bloom_frag_glsl));
    
    _myBloomShader = cclib::GLSLShader::create(
//    _myBloomShader = cclib::Shader::create(
////                                      CCIOUtil.classPath(this, "shader/bloom_vert.glsl"),
////                                      CCIOUtil.classPath(this, "shader/bloom_frag.glsl")
////                                           "shader/bloom.vp","shader/bloom.fp"
//                                               "shader/bloom_vert.glsl","shader/bloom_frag.glsl"
                                               vfiles,ffiles
                                      );
//    _myBloomShader->load();
    
    _myWidth = theWidth;
    _myHeight = theHeight;
}

void cclib::GPUBloom::start()
{
//    if(!_cApplyBloom)return;
    //_myRenderTexture->beginDraw();
    _myRenderTexture->bindFBO();
}

void cclib::GPUBloom::end()
{
//    if(!_cApplyBloom)return;
    //_myRenderTexture->endDraw();
    _myRenderTexture->releaseFBO();
    
//    cclib::Graphics::color(0,1,1, 1.0f);
//    cclib::Graphics::clear();
    
//    cclib::Graphics::clearColor(0);
//    cclib::Graphics::clear();
//    cclib::Graphics::color(1,1,1);
    
    
#if 0
////    if(!_cDebugBloom) {
////        g.image(_myRenderTexture.attachment(0), -_myWidth/2, -_myHeight/2);
////        g.blend(CCBlendMode.ADD);
////    }
//
//    cclib::Graphics::image(_myRenderTexture->attachment(0), 0,0,_myWidth, _myHeight); // TEST
    cclib::Graphics::image(_myRenderTexture->attachment(0), -_myWidth/2., -_myHeight/2., _myWidth, _myHeight); // HALFSCREEN TEST
//    cclib::Graphics::image(_myRenderTexture->attachment(0), -_myWidth, -_myHeight, _myWidth*2, _myHeight*2); // FULLSCREEN

    return;
    
#else

//    _myBlur->setRadius(_cBlurRadius);
    
//    _myBlur->beginDraw();
//    cclib::Graphics::clear();
//    cclib::Graphics::image(_myRenderTexture->attachment(0), -_myWidth, -_myHeight, _myWidth*2, _myHeight*2); // FULLSCREEN
//    _myBlur->endDraw();
    
    cclib::Graphics::clear();

#if 0
    
    cclib::Graphics::image(_myBlur->blurredTexture(), -_myWidth, -_myHeight, _myWidth*2, _myHeight*2); // FULLSCREEN
//    cclib::Graphics::image(_myBlur->blurredTexture(), -_myWidth/2., -_myHeight/2., _myWidth, _myHeight); // HALFSCREEN TEST
//    cclib::Graphics::image(_myRenderTexture1->attachment(0), -_myWidth/2., -_myHeight/2., _myWidth, _myHeight); // HALFSCREEN TEST
    
#else
    _myBloomShader->start();
    
//    glActiveTexture(GL_TEXTURE0);
//    _myRenderTexture->attachment(0)->bind();
    
    _myBloomShader->setUniform1i("texture", 0);
    _myBloomShader->setUniform1f("highlightRange", _cHighlightColor);
    _myBloomShader->setUniform1f("highlightScale", _cHighlightScale);
    _myBloomShader->setUniform1f("highlightPow", _cHighlightPow);
//    cclib::Graphics::image(_myBlur->blurredTexture(), -_myWidth/2, -_myHeight/2, _myWidth, _myHeight);
    //cclib::Graphics::image(_myRenderTexture->attachment(0), -_myWidth/2., -_myHeight/2., _myWidth, _myHeight); // HALFSCREEN TEST
    cclib::Graphics::image(_myRenderTexture->attachment(0), -_myWidth, -_myHeight, _myWidth*2, _myHeight*2); // FULLSCREEN
//    cclib::Graphics::image(_myBlur->blurredTexture(), -_myWidth, -_myHeight, _myWidth*2, _myHeight*2); // FULLSCREEN
    
//    _myRenderTexture->attachment(0)->unbind();
    
    _myBloomShader->end();
#endif
    
#endif
    
//    cclib::Graphics::blend();
    
//    cclib::Graphics::image(_myRenderTexture1->attachment(0), -_myWidth/2., -_myHeight/2., _myWidth, _myHeight); // HALFSCREEN
}

//public void startBlur(CCGraphics g) {
//    _myRenderTexture.beginDraw();
//}
//
//public void endBlur(CCGraphics g) {
//    _myRenderTexture.endDraw();
//    
//    _myBlur.radius(_cBlurRadius2);
//    g.color(255);
//    _myBlur.beginDraw(g);
//    g.clear();
//    g.image(_myRenderTexture.attachment(0), -_myWidth/2, -_myHeight/2);
//    _myBlur.endDraw(g);
//    
//}

