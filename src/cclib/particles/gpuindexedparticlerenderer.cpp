//
//  gpuindexedparticlerenderer.cpp
//  Project
//
//  Created by Frank Eickhoff on 18.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuindexedparticlerenderer.h"
#include "gl/graphics.h"
#include "gl/shaderbuffer.h"
#include "stringified_shaders/indexed_display_vertex.glsl.h"
#include "stringified_shaders/indexed_display_fragment.glsl.h"
#include "stringified_shaders/sorted_display_vertex.glsl.h"
#include "stringified_shaders/sorted_display_fragment.glsl.h"

cclib::GPUIndexedParticleRenderer::GPUIndexedParticleRenderer()
: cclib::GPUParticleRenderer(),
_myPointsize(1)
{
    //printf("%s\n",__PRETTY_FUNCTION__);
    
    std::vector<std::string> vfiles, ffiles;
    
    vfiles.push_back(indexed_display_vertex_glsl);
    ffiles.push_back(indexed_display_fragment_glsl);
    
//    vfiles.push_back(sorted_display_vertex_glsl);
//    ffiles.push_back(sorted_display_fragment_glsl);
    
    //    super(theParticles, theStart, theNumberParticles);
    _myShader = cclib::GLSLShader::create(vfiles, ffiles);
//    _myShader->load();
}

//public CCGPUIndexedParticleRenderer(String theVertexShader, String theFragmentShader) {
//    _myShader = new CCGLSLShader(theVertexShader, theFragmentShader);
//    _myShader.load();
//}
//
//public CCGPUIndexedParticleRenderer() {
//    this(
//         CCIOUtil.classPath(CCGPUDisplayShader.class, "shader/indexed/indexed_display_vertex.glsl"),
//         CCIOUtil.classPath(CCGPUDisplayShader.class, "shader/indexed/indexed_display_fragment.glsl")
//         );
//}

void cclib::GPUIndexedParticleRenderer::setup(cclib::GPUParticles* theParticles) {
    
    //printf("%s\n",__PRETTY_FUNCTION__);
    
    _myParticles = cclib::GPUParticlesPtr(theParticles);
    _myMesh = cclib::VBOMesh::create(GL_POINTS, _myParticles->size());
    _myMesh->prepareVertexData(_myParticles->size(), 3);
    
    for(int y = 0; y < theParticles->height();y++) {
        for(int x = 0; x < theParticles->width();x++) {
            _myMesh->addVertex(x,y,0);
            _myMesh->addTextureCoords(
//            _myParticles->renderer()->mesh()->addTextureCoords(
                            0,
                            ((float)x + 0.5f) / theParticles->width(),
                            ((float)y + 0.5f) / theParticles->height()
                            );
        }
    }
}

void cclib::GPUIndexedParticleRenderer::update(float theDeltaTime) {
    // _myMesh.vertices(_myParticles.positions());
}

void cclib::GPUIndexedParticleRenderer::draw(){

    //printf("%s\n",__PRETTY_FUNCTION__);
    
//    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    _myShader->start();
    cclib::Graphics::texture(0, _myParticles->dataBuffer()->attachment(0));
    cclib::Graphics::texture(1, _myParticles->dataBuffer()->attachment(1));
    cclib::Graphics::texture(3, _myParticles->dataBuffer()->attachment(3));
    _myShader->setUniform1i("positions", 0);
    _myShader->setUniform1i("infos", 1);
    _myShader->setUniform1i("colors", 3);
    
    _myShader->setUniform1f("pointSize", _myPointsize);
    _myShader->setUniform1f("minPointSize", 0.001);
    _myShader->setUniform1f("maxPointSize", 2.5);
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float height = viewport[3];
    
    _myShader->setUniform1f("tanHalfFOV", tan(DEFAULT_FOV) * height);
    
    _myMesh->draw();
    
    cclib::Graphics::noTexture();
    _myShader->end();

//    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

//void cclib::GPUIndexedParticleRenderer::pointSizeClamp(float theMinPointSize, float theMaxPointSize) {
//    //		if(_myDisplayShader == null)_myDisplayShader = new CCGPUDisplayShader();
//    //		_myDisplayShader.minPointSize(theMinPointSize);
//    //		_myDisplayShader.maxPointSize(theMinPointSize);
//}
