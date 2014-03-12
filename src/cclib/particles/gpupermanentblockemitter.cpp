
#include "gpupermanentblockemitter.h"

#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "gl/bufferobject.h"
#include "math/math.h"
#include "particles/gpuparticle.h"
#include "gl/cgshader.h"
#include "gl/shaderbuffer.h"
#include "particles/gpuparticles.h"
#include "gl/mesh.h"

#define MAX(x, y) ((y > x)?y:x)  

cclib::GPUPermanentBlockEmitter::GPUPermanentBlockEmitter(cclib::GPUParticlesPtr theParticles, int theStart, int theWidth, int theHeight) : 
    cclib::GPUIndexParticleEmitter(theParticles, theStart, theWidth * theHeight),
    _emittedOnce(false), 

    // emitting properties
    _myWidth(theWidth),
    _myHeight(theHeight)
{
}

void cclib::GPUPermanentBlockEmitter::update(float theDeltaTime)
{
    if (_emittedOnce) {
        return;
    }

    // udpate shader parameters
    cclib::GPUIndexParticleEmitter::update(theDeltaTime);
    
    // emit new particles
    float width = (float)_myWidth;
    float height = (float)_myHeight;
    cclib::Color color = cclib::Color(1.0f, 1.0f, 1.0f, 1.0f);
    cclib::Vector3f velocity = cclib::Vector3f(0.0f, 0.0f, 0.0f);
    float lifetime = 10.0f;

    float w_half = width * 0.5f;
    float h_half = height * 0.5f;

    for(int y = 0; y < _myHeight; y++) {
        for(int x = 0; x < _myWidth; x++) {
            cclib::Vector3f position = cclib::Vector3f(x - w_half, y - h_half, y); 
            emit(color, position, velocity, lifetime, true);
        }
    }

    // emit only once
    if (width != 0 && height != 0) {
        _emittedOnce = true; 
    }
}
