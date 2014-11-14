#ifndef __CCLIB_HEADER_DEFINED__
#define __CCLIB_HEADER_DEFINED__

#ifdef USE_TR1
#include <tr1/memory>
#define CC_PTR std::tr1::shared_ptr
#else
#include <memory>
#define CC_PTR std::shared_ptr
#endif

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
typedef CC_PTR<FrameBufferObjectAttributes> FrameBufferObjectAttributesPtr; 

class FrameBufferObject; 
typedef CC_PTR<FrameBufferObject> FrameBufferObjectPtr; 

class PixelStorageModes;
typedef CC_PTR<PixelStorageModes> PixelStorageModesPtr;

class CGShader;
typedef CC_PTR<CGShader> CGShaderPtr;

class GLSLShader;
typedef CC_PTR<GLSLShader> GLSLShaderPtr;
    
class PBO;
typedef CC_PTR<PBO> PBOPtr;

class ShaderBuffer;
typedef CC_PTR<ShaderBuffer> ShaderBufferPtr;

class RenderBuffer;
typedef CC_PTR<RenderBuffer> RenderBufferPtr;

class Camera;
typedef CC_PTR<Camera> CameraPtr;

class Viewport;
typedef CC_PTR<Viewport> ViewportPtr;

class TextureAttributes; 
typedef CC_PTR<TextureAttributes> TextureAttributesPtr; 

class Texture;
typedef CC_PTR<Texture> TexturePtr;

class TextureData;
typedef CC_PTR<TextureData> TextureDataPtr;

class Color;
typedef CC_PTR<Color> ColorPtr; 

class Texture2D;
typedef CC_PTR<Texture2D> Texture2DPtr;

class GPUUpdateShader;
typedef CC_PTR<GPUUpdateShader> GPUUpdateShaderPtr;

class GPUParticles;
typedef CC_PTR<GPUParticles> GPUParticlesPtr;

class GPUParticle;
typedef CC_PTR<GPUParticle> GPUParticlePtr;

class GPUForce;
typedef CC_PTR<GPUForce> GPUForcePtr;

class GPUConstraint;
typedef CC_PTR<GPUConstraint> GPUConstraintPtr;

class GPUImpulse;
typedef CC_PTR<GPUImpulse> GPUImpulsePtr;

class GPUParticleEmitter;
typedef CC_PTR<GPUParticleEmitter> GPUParticleEmitterPtr;

class GPUParticleRenderer;
typedef CC_PTR<GPUParticleRenderer> GPUParticleRendererPtr;

class GPUNoise;
typedef CC_PTR<GPUNoise> GPUNoisePtr;

class Mesh;
typedef CC_PTR<Mesh> MeshPtr;

class VBOMesh;
typedef CC_PTR<VBOMesh> VBOMeshPtr;

class BufferObject;
typedef CC_PTR<BufferObject> BufferObjectPtr;

class Buffer;
typedef CC_PTR<Buffer> BufferPtr;
    
class ParticleWaitingList;
typedef CC_PTR<ParticleWaitingList> ParticleWaitingListPtr;

class GPUIndexParticleEmitter;
typedef CC_PTR<GPUIndexParticleEmitter> GPUIndexParticleEmitterPtr;

class GPUDummyRenderer;
typedef CC_PTR<GPUDummyRenderer> GPUDummyRendererPtr;

class GPUIndexedParticleRenderer;
typedef CC_PTR<GPUIndexedParticleRenderer> GPUIndexedParticleRendererPtr;
	
class GPUIndexParticleCurveEmitter;
typedef CC_PTR<GPUIndexParticleCurveEmitter> GPUIndexParticleCurveEmitterPtr;

class GPUSimpleEmitter;
typedef CC_PTR<GPUSimpleEmitter> GPUSimpleEmitterPtr;

class GPUPermanentBlockEmitter;
typedef CC_PTR<GPUPermanentBlockEmitter> GPUPermanentBlockEmitterPtr;

class GPUCurveLineEmitter;
typedef CC_PTR<GPUCurveLineEmitter> GPUCurveLineEmitterPtr;

class GPUGBufferEmitter;
typedef CC_PTR<GPUGBufferEmitter> GPUGBufferEmitterPtr;
		
class GPUParticlePointRenderer;
typedef CC_PTR<GPUParticlePointRenderer> GPUParticlePointRendererPtr;

class GPUDisplayShader;
typedef CC_PTR<GPUDisplayShader> GPUDisplayShaderPtr;

class GPUForceField;
typedef CC_PTR<GPUForceField> GPUForceFieldPtr;

class GPUCurveField;
typedef CC_PTR<GPUCurveField> GPUCurveFieldPtr;

class GPUCurveLine;
typedef CC_PTR<GPUCurveLine> GPUCurveLinePtr;

class GPUGravity;
typedef CC_PTR<GPUGravity> GPUGravityPtr;

class GPUAttractor;
typedef CC_PTR<GPUAttractor> GPUAttractorPtr;

class GPUViscousDrag;
typedef CC_PTR<GPUViscousDrag> GPUViscousDragPtr;

class GPUCombinedForce;
typedef CC_PTR<GPUCombinedForce> GPUCombinedForcePtr;

class GPUTargetForce;
typedef CC_PTR<GPUTargetForce> GPUTargetForcePtr;
    
class GPUTargetSetup;
typedef CC_PTR<GPUTargetSetup> GPUTargetSetupPtr;
    
class GPUTimeForceBlend;
typedef CC_PTR<GPUTimeForceBlend> GPUTimeForceBlendPtr;

class GPUYForceBlend;
typedef CC_PTR<GPUYForceBlend> GPUYForceBlendPtr;
    
class GPUBloom;
typedef CC_PTR<GPUBloom> GPUBloomPtr;
	
class SeparateGaussianBlur;
typedef CC_PTR<SeparateGaussianBlur> SeparateGaussianBlurPtr;

class GPUParticleSort;
typedef CC_PTR<GPUParticleSort> GPUParticleSortPtr;
	
class GPUParticles;
typedef CC_PTR<GPUParticles> GPUParticlesPtr;

class GPUPlaneConstraint;
typedef CC_PTR<GPUPlaneConstraint> GPUPlaneConstraintPtr;
	
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
