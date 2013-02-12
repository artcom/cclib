#ifndef __CCLIB_HEADER_DEFINED__
#define __CCLIB_HEADER_DEFINED__

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <tr1/memory>
#include <GL/glew.h>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <Exception.h>
#include <math/vec2.h>
#include <math/vec3.h>

namespace cclib {

class FrameBufferObjectAttributes;
typedef std::tr1::shared_ptr<FrameBufferObjectAttributes> FrameBufferObjectAttributesPtr; 

class FrameBufferObject; 
typedef std::tr1::shared_ptr<FrameBufferObject> FrameBufferObjectPtr; 

class PixelStorageModes;
typedef std::tr1::shared_ptr<PixelStorageModes> PixelStorageModesPtr;

class Shader;
typedef std::tr1::shared_ptr<Shader> ShaderPtr;

class PBO;
typedef std::tr1::shared_ptr<PBO> PBOPtr;

class ShaderTexture;
typedef std::tr1::shared_ptr<ShaderTexture> ShaderTexturePtr;

class TextureAttributes; 
typedef std::tr1::shared_ptr<TextureAttributes> TextureAttributesPtr; 

class Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;

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
	
class GPUParticlePointRenderer;
typedef std::tr1::shared_ptr<GPUParticlePointRenderer> GPUParticlePointRendererPtr;

class GPUDisplayShader;
typedef std::tr1::shared_ptr<GPUDisplayShader> GPUDisplayShaderPtr;

class GPUForceField;
typedef std::tr1::shared_ptr<GPUForceField> GPUForceFieldPtr;

};


// // #include <gl/graphics.h>
// #include <gl/pixelstoragemodes.h>
// #include <gl/texture.h>
// #include <gl/texture2d.h>
// #include <gl/shader.h>
// #include <gl/framebufferobject.h>
// #include <gl/shadertexture.h>
// 
// #include <particles/gpuparticle.h>
// #include <particles/gpuupdateshader.h>
// #include <particles/gpuforce.h>
// #include <particles/gpuparticles.h>
// #include <particles/gpuconstraint.h>
// #include <particles/gpuimpulse.h>
// // #include <particles/gpudisplayshader.h>
// // #include <particles/gpuparticlepointrenderer.h>
// // #include <particles/gpuindexparticleemitter.h>

#endif
