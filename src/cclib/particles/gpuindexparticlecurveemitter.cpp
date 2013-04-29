//
//  gpuindexparticlecurveemitter.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuindexparticlecurveemitter.h"

#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "gl/bufferobject.h"
#include "math/math.h"
#include "particles/gpuparticle.h"
#include "gl/cgshader.h"
#include "gl/shaderbuffer.h"
#include "particles/gpuparticles.h"
#include "gl/mesh.h"

//cclib::GPUIndexParticleCurveEmitter::GPUIndexParticleCurveEmitter()
//: cclib::GPUIndexParticleEmitter()
//{
//    printf("%s\n",__PRETTY_FUNCTION__);
//}

cclib::GPUIndexParticleCurveEmitter::GPUIndexParticleCurveEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theNumberParticles)
: cclib::GPUIndexParticleEmitter(theParticles, theStart,theNumberParticles),
_myOffset(0),
_myScale(1),
_myOutputScale(1),
_myRadius(1),
_mySpeed(1)

{
    printf("%s\n",__PRETTY_FUNCTION__);
    
    std::vector<std::string> vfiles, ffiles;
    ffiles.push_back(std::string(simplex_fp));
    ffiles.push_back(std::string(curvefield_emit_fp));
    
//    super(theParticles, theStart, theNumberParticles);
    _myCurveEmitShader = cclib::CGShader::create(
                            //"shader/simplex.fp","shader/emit/curvefield_emit.fp"
                            vfiles, ffiles
                                               );

    _myRadiusParameter = _myCurveEmitShader->fragmentParameter("radius");
    _myOffsetParameter = _myCurveEmitShader->fragmentParameter("offset");
    _myScaleParameter = _myCurveEmitShader->fragmentParameter("scale");
    _myOutputScaleParameter = _myCurveEmitShader->fragmentParameter("outputScale");
    _myCurveEmitShader->load();

    cclib::GPUNoise::attachFragmentNoise((cclib::CGShader*)_myCurveEmitShader.get());
}

//public CCGPUIndexParticleCurveEmitter(CCGPUParticles theParticles) {
//    super(theParticles);
//}

//public void scale(float theScale) {
//    _myScale = theScale;
//}
//
//public void outputScale(float theOutputScale) {
//    _myOutputScale = theOutputScale;
//}

//public void radius(float theRadius) {
//    _myRadius = theRadius;
//}
//
//public void speed(float theSpeed) {
//    _mySpeed = theSpeed;
//}

void cclib::GPUIndexParticleCurveEmitter::update(float theDeltaTime)
{
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
    
    _myOffset += theDeltaTime * _mySpeed;
    
    _myCurveEmitShader->parameter(_myOffsetParameter, _myOffset);
    _myCurveEmitShader->parameter(_myOutputScaleParameter, _myOutputScale);
    _myCurveEmitShader->parameter(_myScaleParameter, _myScale);
    _myCurveEmitShader->parameter(_myRadiusParameter, _myRadius);
}

//void cclib::GPUIndexParticleCurveEmitter::fillPositionData(FloatBuffer theBuffer, List<CCGPUParticle> theParticles)
//void cclib::GPUIndexParticleCurveEmitter::fillPositionData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticle> theParticles)
void cclib::GPUIndexParticleCurveEmitter::fillPositionData(cclib::BufferPtr theBuffer)
{
//    cclib::GPUIndexParticleEmitter::fillPositionData(theBuffer); return;
    
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++)
    {
        cclib::GPUParticlePtr myParticle = _myAllocatedParticles[i];
        
        theBuffer->data()[i * 3 + 0] = myParticle->position()->x();
        theBuffer->data()[i * 3 + 1] = cclib::random<float>();
        theBuffer->data()[i * 3 + 2] = cclib::random<float>(2.*M_PI);
    }
    
//    int i = 0;
//    for (CCGPUParticle myParticle:theParticles){
//        _myFillArray[i * 3 + 0] = myParticle.position().x;
//        _myFillArray[i * 3 + 1] = CCMath.random();
//        _myFillArray[i * 3 + 2] = CCMath.random(CCMath.TWO_PI);
//        i++;
//    }
//    theBuffer.put(_myFillArray, 0, theParticles.size() * 3);
}

//void cclib::GPUIndexParticleCurveEmitter::transferEmitData()
void cclib::GPUIndexParticleCurveEmitter::transferData()
{
//    cclib::GPUIndexParticleEmitter::transferData();return;

    _myParticles->dataBuffer()->beginDraw();
    _myCurveEmitShader->start();
    
    _myEmitMesh->draw();
    
    _myCurveEmitShader->end();
    _myParticles->dataBuffer()->endDraw();
}
