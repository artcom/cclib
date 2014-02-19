// #include <cclib.h>
#include "capi/capi.h"
#include "capi/particleswrapper.h"

// capi calls to simplify binding to other languages

unity_plugin::ParticlesWrapperPtr _particlesWrapper;
FuncPtr DebugLog = NULL;

// setup base particle system
int cclib_initializeParticleSystem() {
    try {
        _particlesWrapper = unity_plugin::ParticlesWrapper::create();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    
    return 0;
}

int cclib_addComponent(char * componentName, char * instanceName) {
    try {
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterFloat(char * parameterName, float parameterValue) {
    try {
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_setupParticleSystem(void* texturePointer) {
    try {
        _particlesWrapper->setup(texturePointer);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }

    return 0;
};

int cclib_updateSimulation() {
    try {
        _particlesWrapper->updateSimulation();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_copyResults() {
    try {
        _particlesWrapper->copyResults();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

// and then teardown again
int cclib_teardownParticleSystem() {
    try {
        _particlesWrapper = unity_plugin::ParticlesWrapperPtr();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

void SetDebugFunction(FuncPtr fp) {
    DebugLog = fp;
    DebugLog("Pointer set.");
}

extern "C" void UnityRenderEvent (int eventID)
{
	cclib_updateSimulation();
    cclib_copyResults();   
}

extern "C" void UnitySetGraphicsDevice (void* device, int deviceType, int eventType)
{
	// If we've got an OpenGL device, remember device type. There's no OpenGL
	// "device pointer" to remember since OpenGL always operates on a currently set
	// global context.
	if (deviceType == kGfxRendererOpenGL)
	{
        glewInit();
    }
}
