#include "gpugbufferemitter.h"

#include <stringified_shaders/emit/gbuffer_emit.fp.h>
#include <gl/shaderbuffer.h>
#include <math/math.h>
#include <particles/gpuparticle.h>
#include <gl/cgshader.h>
#include <gl/shaderbuffer.h>
#include <particles/gpuparticles.h>
#include <gl/mesh.h>
#include <gl/graphics.h>

cclib::GPUGBufferEmitter::GPUGBufferEmitter(cclib::GPUParticlesPtr theParticles) : 
    cclib::GPUIndexParticleEmitter(),
    _myParticles(theParticles),
    _myPositionBuffer(Property_<float>::create("positionBuffer", 0)),
    _myColorBuffer(Property_<float>::create("colorBuffer", 0)),
    _myMaxLifeTime(Property_<float>::create("maxLifeTime", 12.0f)),
    _myMinLifeTime(Property_<float>::create("minLifeTime", 10.0f)),
    _myEmitProbability(Property_<float>::create("emitProb", 0.5f)),
    _myPositionJitter(Property_<float>::create("positionJitter", 1.0f))
{
    registerProperty(_myPositionBuffer);
    registerProperty(_myColorBuffer);
    registerProperty(_myMaxLifeTime);
    registerProperty(_myMinLifeTime);
    registerProperty(_myEmitProbability);
    registerProperty(_myPositionJitter);
   
    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(gbuffer_emit_fp);
    _myEmitShader = cclib::CGShader::create(vfiles, ffiles);

    _myPositionTextureParameter = _myEmitShader->fragmentParameter("positionTexture");
    _myInfoTextureParameter = _myEmitShader->fragmentParameter("infoTexture");
    _myVelocityTextureParameter = _myEmitShader->fragmentParameter("velocityTexture");
    _myColorTextureParameter = _myEmitShader->fragmentParameter("colorTexture");
    
    _myGeometryTextureParameter = _myEmitShader->fragmentParameter("geometryTexture");
    _myGeometryColorTextureParameter = _myEmitShader->fragmentParameter("geometryColorTexture");

    _myGBufferSizeParameter = _myEmitShader->fragmentParameter("gBufferSize");
    _myRandomSeedParameter = _myEmitShader->fragmentParameter("randomSeed");
    _myEmitProbabilityParameter = _myEmitShader->fragmentParameter("emitProb");
    _myMaxLifeTimeParameter = _myEmitShader->fragmentParameter("maxLifeTime");
    _myMinLifeTimeParameter = _myEmitShader->fragmentParameter("minLifeTime");
    _myPositionJitterParameter = _myEmitShader->fragmentParameter("positionJitter");

    _myEmitShader->load();
}

void cclib::GPUGBufferEmitter::setData()
{
}

void cclib::GPUGBufferEmitter::update(float)
{
    _myEmitShader->start();
    _myEmitShader->texture(_myPositionTextureParameter, _myParticles->dataBuffer()->attachment(0)->id());
    _myEmitShader->texture(_myInfoTextureParameter, _myParticles->dataBuffer()->attachment(1)->id());
    _myEmitShader->texture(_myVelocityTextureParameter, _myParticles->dataBuffer()->attachment(2)->id());
    _myEmitShader->texture(_myColorTextureParameter, _myParticles->dataBuffer()->attachment(3)->id());

    GLuint positionTexture = (GLuint)(size_t)static_cast<int>(_myPositionBuffer->getValue<float>());
    GLuint colorTexture    = (GLuint)(size_t)static_cast<int>(_myColorBuffer->getValue<float>());

    glEnable(GL_TEXTURE_2D);
    // glEnable(GL_TEXTURE_RECTANGLE);
    if (positionTexture != 0 && colorTexture != 0) {
        _myEmitShader->texture(_myGeometryTextureParameter, positionTexture);
        _myEmitShader->texture(_myGeometryColorTextureParameter, colorTexture); 
    }

    _myEmitShader->parameter(_myRandomSeedParameter, cclib::random<float>(100.f), cclib::random<float>(100.f),  cclib::random<float>(3000,10000));
    _myEmitShader->parameter(_myMinLifeTimeParameter, _myMinLifeTime->getValue<float>());
    _myEmitShader->parameter(_myMaxLifeTimeParameter, _myMaxLifeTime->getValue<float>());
    _myEmitShader->parameter(_myEmitProbabilityParameter, _myEmitProbability->getValue<float>());
    _myEmitShader->parameter(_myPositionJitterParameter, _myPositionJitter->getValue<float>());

    glBindTexture(GL_TEXTURE_2D, positionTexture);
    int gWidth, gHeight;
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &gWidth);
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &gHeight);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // _myEmitShader->parameter(_myGBufferSizeParameter, gWidth, gHeight); 
    
    _myParticles->destinationDataTexture()->draw();
    cclib::Graphics::checkError();

    _myEmitShader->end();
    _myParticles->swapDataTextures();
}
