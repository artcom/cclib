// #include <cclib.h>
#include "capi/capi.h"
#include "capi/particleswrapper.h"

#include <cclib.h>

// capi calls to simplify binding to other languages

unity_plugin::ParticlesWrapperPtr _particlesWrapper;
FuncPtr DebugLog = NULL;
float _deltaT = 0.016667f;

// setup base particle system
int cclib_initializeParticleSystem() {
    try {
        _particlesWrapper = unity_plugin::ParticlesWrapper::create();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    
    return 0;
}

int cclib_addForce(char * forceName, char * instanceName) {
    try {
        std::string name(forceName);
        std::string iname(instanceName);
        _particlesWrapper->addForce(name, iname); 
    
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_addEmitter(char * emitterName, char * instanceName) {
    try {
        std::string name(emitterName);
        std::string iname(instanceName);
        _particlesWrapper->addEmitter(name, iname); 
    
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}
    
int cclib_updateParameterFloat(char * componentName, char * parameterName, float parameterValue) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        _particlesWrapper->setParameter<float>(name, pname, parameterValue);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterVector3(char * componentName, char * parameterName, float x, float y, float z) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        cclib::Vector3f value(x, y, z);

        _particlesWrapper->setParameter<cclib::Vector3f>(name, pname, value);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterComponentReference(char * componentName, char * parameterName, char * referenceName) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        std::string value(referenceName);
        
        _particlesWrapper->setComponentReference(name, pname, value);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterInt(char * componentName, char * parameterName, int parameterValue) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        _particlesWrapper->setParameter<int>(name, pname, parameterValue);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterVectorOfVector2(char * componentName, char * parameterName, 
       float x, float y, int index) 
{
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        // slow but simple.
        _particlesWrapper->setVector2fIndexParameter(name, pname, x, y, index);
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

int cclib_setInfoTexture(void* texturePointer) {
    try {
        _particlesWrapper->setInfoTexture(texturePointer);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }

    return 0;
};

int cclib_setColorTexture(void* texturePointer) {
    try {
        _particlesWrapper->setColorTexture(texturePointer);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }

    return 0;
};

int cclib_updateSimulation() {
    try {
        _particlesWrapper->updateSimulation(_deltaT);
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

int cclib_setSimulationTime(float theDeltaTime) {
    _deltaT = theDeltaTime;
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
