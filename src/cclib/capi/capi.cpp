// #include <cclib.h>
#include "capi/capi.h"
#include "capi/particleswrapper.h"

#include <cclib.h>

// capi calls to simplify binding to other languages

std::map<int, unity_plugin::ParticlesWrapperPtr> _particlesWrappers;

StringFuncPtr DebugLog = ToStdOut;
VoidFuncPtr InitializeFunction = Pass;
VoidFuncPtr UpdateParameterFunction = Pass;

int pCounter = 0;

float _deltaT = 0.016667f;

// setup base particle system
int cclib_initializeParticleSystem() {
    pCounter++;
    
    try {
        unity_plugin::ParticlesWrapperPtr particlesWrapper = unity_plugin::ParticlesWrapper::create();
        _particlesWrappers[pCounter] = particlesWrapper;
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    
    return pCounter;
}

int cclib_addForce(int wrapper, char * forceName, char * instanceName) {
    try {
        std::string name(forceName);
        std::string iname(instanceName);
        _particlesWrappers[wrapper]->addForce(name, iname);
    
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_addCombinedForce(int wrapper, char * forceName, char * instanceName, char * force1, char * force2) {
    try {
        std::string name(forceName);
        std::string force1Name(force1);
        std::string force2Name(force2);

        std::string iname(instanceName);
        _particlesWrappers[wrapper]->addCombinedForce(name, iname, force1Name, force2Name);
    
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_addEmitter(int wrapper, char * emitterName, char * instanceName) {
    try {
        std::string name(emitterName);
        std::string iname(instanceName);
        _particlesWrappers[wrapper]->addEmitter(name, iname);
    
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}
    
int cclib_updateParameterFloat(int wrapper, char * componentName, char * parameterName, float parameterValue) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        _particlesWrappers[wrapper]->setParameter<float>(name, pname, parameterValue);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterVector3(int wrapper, char * componentName, char * parameterName, float x, float y, float z) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        cclib::Vector3f value(x, y, z);

        _particlesWrappers[wrapper]->setParameter<cclib::Vector3f>(name, pname, value);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterComponentReference(int wrapper, char * componentName, char * parameterName, char * referenceName) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        std::string value(referenceName);
        
        _particlesWrappers[wrapper]->setComponentReference(name, pname, value);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterInt(int wrapper, char * componentName, char * parameterName, int parameterValue) {
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        _particlesWrappers[wrapper]->setParameter<int>(name, pname, parameterValue);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterVectorOfVector2(int wrapper, char * componentName, char * parameterName, 
       float x, float y, int index) 
{
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        // slow but simple.
        _particlesWrappers[wrapper]->setVector2fIndexParameter(name, pname, x, y, index);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_updateParameterVectorOfVector3(int wrapper, char * componentName, char * parameterName, 
       float x, float y, float z, int index) 
{
    try {
        std::string name(componentName);
        std::string pname(parameterName);
        
        // slow but simple.
        _particlesWrappers[wrapper]->setVector3fIndexParameter(name, pname, x, y, z, index);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_setupParticleSystem(int wrapper, void* texture) {
    try {
        _particlesWrappers[wrapper]->setup(texture);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
};

int cclib_setInfoTexture(int wrapper, void* texture) {
    try {
        _particlesWrappers[wrapper]->setInfoTexture(texture);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }

    return 0;
};

int cclib_setColorTexture(int wrapper, void* texture) {
    try {
        _particlesWrappers[wrapper]->setColorTexture(texture);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }

    return 0;
};

int cclib_updateSimulation(int wrapper) {
    try {
        _particlesWrappers[wrapper]->updateSimulation(_deltaT);
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_copyResults(int wrapper) {
    try {
        _particlesWrappers[wrapper]->copyResults();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

// and then teardown again
int cclib_teardownParticleSystem(int wrapper) {
    try {
        _particlesWrappers[wrapper]->teardown();
        _particlesWrappers[wrapper] = unity_plugin::ParticlesWrapperPtr();
    } catch (cclib::Exception & e) {
        DebugLog(e.what());
    }
    return 0;
}

int cclib_setSimulationTime(int wrapper, float theDeltaTime) {
    _deltaT = theDeltaTime;
    return 0;
}

void SetDebugFunction(StringFuncPtr fp) {
    DebugLog = fp;
    DebugLog("Pointer set.");
}

void SetInitializeFunction(int wrapper, VoidFuncPtr fp) {
    InitializeFunction = fp;
    DebugLog("Initialization Function Pointer set.");
}

void SetUpdateParameterFunction(int wrapper, VoidFuncPtr fp) {
    UpdateParameterFunction = fp;
    DebugLog("UpdateParameter Function Pointer set.");
}

void ToStdOut(const char * message) {
    std::cout << "Debug: " << message << std::endl;
}

void Pass() {
}

extern "C" void UnityRenderEvent (int eventID)
{
    // calls c# function to update the particle system parameters.
    // It has to be done that way because the update must happen in the render thread, 
    // which we can only access the the UnityRenderEvent function
    // InitializeFunction(); 
    // UpdateParameterFunction();
	
    cclib_updateSimulation(eventID);
    cclib_copyResults(eventID);   
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
