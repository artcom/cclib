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
    int cclib_setupParticleSystem(int wrapper, void * texturePointer);
    int cclib_teardownParticleSystem(int wrapper);
    
    int cclib_updateParameterFloat(int wrapper, char * componentName, char * parameterName, float parameterValue);
    int cclib_updateParameterVector3(int wrapper, char * componentName, char * parameterName, float x, float y, float z);
    int cclib_updateParameterInt(int wrapper, char * componentName, char * parameterName, float parameterValue);
    int cclib_updateParameterVectorOfVector2(int wrapper, char * componentName, char * parameterName, 
                 float x, float y, int index); 
    int cclib_updateParameterVectorOfVector3(int wrapper, char * componentName, char * parameterName, 
                 float x, float y, float z, int index); 
    int cclib_updateParameterComponentReference(int wrapper, char * componentName, char * parameterName, char * referenceName);
    
    int cclib_addForce(int wrapper, char * forceName, char * instanceName);
    int cclib_addCombinedForce(int wrapper, char * forceName, char * instanceName, char * force1, char * force2);
    int cclib_addEmitter(int wrapper, char * emitterName, char * instanceName);
    
    int cclib_runParticleSystem(int wrapper);
    int cclib_updateSimulation(int wrapper);
    int cclib_copyResults(int wrapper); // copy simulation results into an external opengl texture id

    int cclib_setInfoTexture(int wrapper, void* texturePointer);
    int cclib_setColorTexture(int wrapper, void* texturePointer);

    int cclib_setSimulationTime(int wrapper, float theDeltaTime);

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
