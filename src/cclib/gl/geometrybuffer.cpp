//
//  GeometryBuffer.cpp
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "geometrybuffer.h"
#include <stringified_shaders/postprocess/geometrybuffer_vertex.glsl.h>
#include <stringified_shaders/postprocess/geometrybuffer_fragment.glsl.h>
#include <math/matrix.h>

cclib::GeometryBuffer::GeometryBuffer(float theWidth, float theHeight)
{
//    CCTextureAttributes myTextureAttributes = new CCTextureAttributes();
//    myTextureAttributes.internalFormat(CCPixelInternalFormat.RGBA32F);
//    myTextureAttributes.filter(CCTextureFilter.NEAREST);
//    myTextureAttributes.wrap(CCTextureWrap.CLAMP);
    
    int theNumberOfTextures = 3;
    int theNumberOfBits = 32, theNumberOfChannels = 4;
    
    cclib::TextureAttributesPtr myTextureAttributes = cclib::TextureAttributes::create(theNumberOfBits, theNumberOfChannels);
    
    cclib::FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create(myTextureAttributes,theNumberOfTextures);
    
//    myAttributes->numberOfSamples = 0;
//    myAttributes->numberOfBits = theNumberOfBits;
//    myAttributes->numberOfChannels = theNumberOfChannels;
//    myAttributes->depthBuffer = true;
    
    
//    CCFrameBufferObjectAttributes myAttributes = new CCFrameBufferObjectAttributes(myTextureAttributes,3);
//    myAttributes.enableDepthBuffer(true);
//    
//    _myRenderTexture = new CCRenderBuffer(g, myAttributes, theWidth, theHeight);

    _myRenderTexture = cclib::FrameBufferObject::create(
                                                        //GL_TEXTURE_2D,
                                                        GL_TEXTURE_RECTANGLE,
                                                        myAttributes, theWidth, theHeight);
    
//    _myShader = new CCGLSLShader(
//                                 CCIOUtil.classPath(this, "geometrybuffer_vertex.glsl"),
//                                 CCIOUtil.classPath(this, "geometrybuffer_fragment.glsl")
//                                 );
//    _myShader.load();

    
    std::vector<std::string> vfiles, ffiles;
    
    vfiles.push_back(std::string(geometrybuffer_vertex_glsl));
    ffiles.push_back(std::string(geometrybuffer_fragment_glsl));
    
    _myShader = cclib::GLSLShader::create(vfiles,ffiles);
    
    
    _myWidth = theWidth;
    _myHeight = theHeight;
}


void cclib::GeometryBuffer::beginDraw()
{
    _myRenderTexture->bindFBO();
    _myShader->start();
    _myShader->setUniform1f( "near", 77.94228
                            );
    _myShader->setUniform1f( "far", 794.2285
                            );
    _myShader->setUniform1i("colorTexture", 0);
    updateMatrix();
}

void cclib::GeometryBuffer::updateMatrix()
{
    Matrix4f m;
    m.loadModelView();
    m.invert();
    
    updateMatrix(m.toFloatBuffer());
}

void cclib::GeometryBuffer::updateMatrix(const std::vector<float>& matrix)
{
    _myShader->setUniformMatrix4fv("inverseView", matrix);
}

void cclib::GeometryBuffer::updateMatrix(const float* matrix)
{
    _myShader->setUniformMatrix4fv("inverseView", matrix);
}

//CCMatrix4f cclib::GeometryBuffer::inverseView(){
//    _myRenderTexture.camera().updateProjectionInfos();
//    return _myRenderTexture.camera().viewMatrix().invert();
//}

void cclib::GeometryBuffer::endDraw()
{
    _myShader->end();
    _myRenderTexture->releaseFBO();
}

//CCRenderBuffer cclib::GeometryBuffer::data() {
//    return _myRenderTexture;
//}
//
//public int width(){
//    return _myWidth;
//}
//
//public int height(){
//    return _myHeight;
//}
//
//public CCTexture2D positions(){
//    return _myRenderTexture.attachment(0);
//}



