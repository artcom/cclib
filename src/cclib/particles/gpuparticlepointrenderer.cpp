
#include <particles/gpuparticlerenderer.h>
#include <particles/gpuparticles.h>
#include <particles/gpudisplayshader.h>
#include <gl/mesh.h>
#include <gl/vbomesh.h>
#include <gl/graphics.h>
#include <gl/shaderbuffer.h>
#include "gpuparticlepointrenderer.h"

using namespace cclib;

GPUParticlePointRenderer::GPUParticlePointRenderer() : 
    GPUParticleRenderer()
{
    _myDisplayShader = GPUDisplayShader::create();
}

GPUParticlePointRendererPtr 
GPUParticlePointRenderer::create() {
    return GPUParticlePointRendererPtr( new GPUParticlePointRenderer() );
}

void 
GPUParticlePointRenderer::setup(GPUParticles * theParticles) {
    _myParticles = GPUParticlesPtr(theParticles);

    _myMesh = VBOMesh::create(GL_POINTS, _myParticles->size());
}

void 
GPUParticlePointRenderer::update(float theDeltaTime) {
    _myMesh->vertices( _myParticles->dataBuffer() );
}

void 
GPUParticlePointRenderer::draw() {
//#warning glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); disabled
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    // _myParticles->dataTexture()->bind(1);
    _myDisplayShader->start();
    // _myDisplayShader->tangHalfFov( CCMath.tan( g.camera().fov() ) * g.height ); // XXX implement camera fov getter/setter and context height
    //_myDisplayShader->tangHalfFov( 1299.0382 );
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    float height = viewport[3];
    _myDisplayShader->tangHalfFov( tan(DEFAULT_FOV) * height );
    
    _myMesh->draw();
    _myDisplayShader->end();
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

VBOMeshPtr 
GPUParticlePointRenderer::mesh(){
    return _myMesh;
}

void 
GPUParticlePointRenderer::pointSize(float thePointSize) {

    _myDisplayShader->pointSize(thePointSize);
}

void 
GPUParticlePointRenderer::pointSizeClamp(float theMinPointSize, float theMaxPointSize) {
    _myDisplayShader->minPointSize(theMinPointSize);
    _myDisplayShader->maxPointSize(theMinPointSize);
}

