#ifndef __CCLIB_HEADER_DEFINED__
#define __CCLIB_HEADER_DEFINED__

#include <math.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>

#include <GL/glew.h>
#include <stdexcept>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/plane3.h>
#include <math/matrix.h>

namespace cclib {

class FrameBufferObjectAttributes;
typedef std::tr1::shared_ptr<FrameBufferObjectAttributes> FrameBufferObjectAttributesPtr; 

class FrameBufferObject; 
typedef std::tr1::shared_ptr<FrameBufferObject> FrameBufferObjectPtr; 

class PixelStorageModes;
typedef std::tr1::shared_ptr<PixelStorageModes> PixelStorageModesPtr;

class CGShader;
typedef std::tr1::shared_ptr<CGShader> CGShaderPtr;

class GLSLShader;
typedef std::tr1::shared_ptr<GLSLShader> GLSLShaderPtr;
    
class PBO;
typedef std::tr1::shared_ptr<PBO> PBOPtr;

class ShaderBuffer;
typedef std::tr1::shared_ptr<ShaderBuffer> ShaderBufferPtr;

class TextureAttributes; 
typedef std::tr1::shared_ptr<TextureAttributes> TextureAttributesPtr; 

class Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;

class TextureData;
typedef std::tr1::shared_ptr<TextureData> TextureDataPtr;

class Color;
typedef std::tr1::shared_ptr<Color> ColorPtr; 

class Texture2D;
typedef std::tr1::shared_ptr<Texture2D> Texture2DPtr;

class GPUUpdateShader;
typedef std::tr1::shared_ptr<GPUUpdateShader> GPUUpdateShaderPtr;

class GPUParticles;
typedef std::tr1::shared_ptr<GPUParticles> GPUParticlesPtr;

class GPUParticle;
typedef std::tr1::shared_ptr<GPUParticle> GPUParticlePtr;

class GPUForce;
typedef std::tr1::shared_ptr<GPUForce> GPUForcePtr;

class GPUConstraint;
typedef std::tr1::shared_ptr<GPUConstraint> GPUConstraintPtr;

class GPUImpulse;
typedef std::tr1::shared_ptr<GPUImpulse> GPUImpulsePtr;

class GPUParticleEmitter;
typedef std::tr1::shared_ptr<GPUParticleEmitter> GPUParticleEmitterPtr;

class GPUParticleRenderer;
typedef std::tr1::shared_ptr<GPUParticleRenderer> GPUParticleRendererPtr;

class GPUNoise;
typedef std::tr1::shared_ptr<GPUNoise> GPUNoisePtr;

class Mesh;
typedef std::tr1::shared_ptr<Mesh> MeshPtr;

class VBOMesh;
typedef std::tr1::shared_ptr<VBOMesh> VBOMeshPtr;

class BufferObject;
typedef std::tr1::shared_ptr<BufferObject> BufferObjectPtr;

class Buffer;
typedef std::tr1::shared_ptr<Buffer> BufferPtr;
    
class ParticleWaitingList;
typedef std::tr1::shared_ptr<ParticleWaitingList> ParticleWaitingListPtr;

class GPUIndexParticleEmitter;
typedef std::tr1::shared_ptr<GPUIndexParticleEmitter> GPUIndexParticleEmitterPtr;

class GPUIndexParticleCurveEmitter;
typedef std::tr1::shared_ptr<GPUIndexParticleCurveEmitter> GPUIndexParticleCurveEmitterPtr;
	
class GPUParticlePointRenderer;
typedef std::tr1::shared_ptr<GPUParticlePointRenderer> GPUParticlePointRendererPtr;

class GPUDisplayShader;
typedef std::tr1::shared_ptr<GPUDisplayShader> GPUDisplayShaderPtr;

class GPUForceField;
typedef std::tr1::shared_ptr<GPUForceField> GPUForceFieldPtr;

class GPUCurveField;
typedef std::tr1::shared_ptr<GPUCurveField> GPUCurveFieldPtr;

class GPUGravity;
typedef std::tr1::shared_ptr<GPUGravity> GPUGravityPtr;

class GPUAttractor;
typedef std::tr1::shared_ptr<GPUAttractor> GPUAttractorPtr;

class GPUViscousDrag;
typedef std::tr1::shared_ptr<GPUViscousDrag> GPUViscousDragPtr;

class GPUCombinedForce;
typedef std::tr1::shared_ptr<GPUCombinedForce> GPUCombinedForcePtr;

class GPUTargetForce;
typedef std::tr1::shared_ptr<GPUTargetForce> GPUTargetForcePtr;
    
class GPUTargetSetup;
typedef std::tr1::shared_ptr<GPUTargetSetup> GPUTargetSetupPtr;
    
class GPUTimeForceBlend;
typedef std::tr1::shared_ptr<GPUTimeForceBlend> GPUTimeForceBlendPtr;
    
class Exception: public std::runtime_error
{
    public:
        Exception(std::string const& msg):
            std::runtime_error(msg)
        {
            std::cout << "Exception:\n" << this->what() << std::endl;
        }
};
    

};

#endif
