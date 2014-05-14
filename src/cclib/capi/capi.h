#ifndef __CCLIB_CAPI_H__
#define __CCLIB_CAPI_H__

// Graphics device identifiers in Unity
enum GfxDeviceRenderer
{
	kGfxRendererOpenGL = 0,          // OpenGL
	kGfxRendererD3D9,                // Direct3D 9
	kGfxRendererD3D11,               // Direct3D 11
	kGfxRendererGCM,                 // Sony PlayStation 3 GCM
	kGfxRendererNull,                // "null" device (used in batch mode)
	kGfxRendererHollywood,           // Nintendo Wii
	kGfxRendererXenon,               // Xbox 360
	kGfxRendererOpenGLES,            // OpenGL ES 1.1
	kGfxRendererOpenGLES20Mobile,    // OpenGL ES 2.0 mobile variant
	kGfxRendererMolehill,            // Flash 11 Stage3D
	kGfxRendererOpenGLES20Desktop,   // OpenGL ES 2.0 desktop variant (i.e. NaCl)
	kGfxRendererCount
};


// Event types for UnitySetGraphicsDevice
enum GfxDeviceEventType {
	kGfxDeviceEventInitialize = 0,
	kGfxDeviceEventShutdown,
	kGfxDeviceEventBeforeReset,
	kGfxDeviceEventAfterReset,
};

typedef void (*StringFuncPtr)( const char * );
typedef void (*VoidFuncPtr)();

extern StringFuncPtr DebugLog;
extern VoidFuncPtr InitializeFunction;
extern VoidFuncPtr UpdateParameterFunction;

// capi calls to simplify binding to other languages
extern "C" {
    int cclib_initializeParticleSystem();
    int cclib_setupParticleSystem(void * texturePointer);
    int cclib_teardownParticleSystem();
    
    int cclib_updateParameterFloat(char * componentName, char * parameterName, float parameterValue);
    int cclib_updateParameterVector3(char * componentName, char * parameterName, float x, float y, float z);
    int cclib_updateParameterInt(char * componentName, char * parameterName, float parameterValue);
    int cclib_updateParameterVectorOfVector2(char * componentName, char * parameterName, 
                 float x, float y, int index); 
    int cclib_updateParameterVectorOfVector3(char * componentName, char * parameterName, 
                 float x, float y, float z, int index); 
    int cclib_updateParameterComponentReference(char * componentName, char * parameterName, char * referenceName);
    
    int cclib_addForce(char * forceName, char * instanceName);
    int cclib_addEmitter(char * emitterName, char * instanceName);
    
    int cclib_runParticleSystem();
    int cclib_updateSimulation();
    int cclib_copyResults(); // copy simulation results into an external opengl texture id

    int cclib_setInfoTexture(void* texturePointer);
    int cclib_setColorTexture(void* texturePointer);

    int cclib_setSimulationTime(float theDeltaTime);

    // Unity specifics
    void SetDebugFunction(StringFuncPtr fp); 
    void SetInitializeFunction(VoidFuncPtr fp); 
    void SetUpdateParameterFunction(VoidFuncPtr fp); 
    
    void UnitySetGraphicsDevice (void* device, int deviceType, int eventType);
    void UnityRenderEvent (int eventID);
    void ToStdOut(const char * message);
    void Pass();
}

#endif
