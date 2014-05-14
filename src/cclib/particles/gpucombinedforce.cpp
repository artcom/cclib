
#include "gpucombinedforce.h"

using namespace cclib; 

GPUCombinedForce::GPUCombinedForce(std::vector<GPUForcePtr> & theForces) :
    GPUForce("CombinedForce"), _myForces(theForces)
{
}

GPUCombinedForcePtr
GPUCombinedForce::create(std::vector<GPUForcePtr> & theForces) {
    return GPUCombinedForcePtr(new GPUCombinedForce(theForces));
}

void 
GPUCombinedForce::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    for(unsigned int i=0; i<_myForces.size(); i++) {
        _myForces[i]->setupParameter(theWidth, theHeight);
    }
}

void
GPUCombinedForce::setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                            int theIndex, int theWidth, int theHeight)
{
    
    std::stringstream ss;
    ss << std::string("forces[");
    ss << theIndex; //add number to the stream
    ss << std::string("]");
    setShader(theParticles, theShader, ss.str(), theWidth, theHeight);
}

void
GPUCombinedForce::setShader(GPUParticlesPtr theParticles, GPUUpdateShaderPtr theShader,
                             std::string theIndex, int theWidth, int theHeight)
{
    _myParticles = GPUParticlesPtr(theParticles);
    _myVelocityShader = GPUUpdateShaderPtr(theShader);
    _myParameterIndex = theIndex;

    _myVelocityShader->checkCGError("Problem creating force.");
    cgConnectParameter(_myVelocityShader->createFragmentParameter(_myShaderTypeName),
                       _myVelocityShader->fragmentParameter(_myParameterIndex));
    
    _myVelocityShader->checkCGError("Problem creating force.");
    
    std::stringstream fp;
    fp << _myParameterIndex << std::string(".forces");
    
    CGparameter myForcesParameter = _myVelocityShader->fragmentParameter(fp.str());
    cgSetArraySize(myForcesParameter, _myForces.size());
    
    int myCounter = 0;
    
    for(unsigned int i=0; i<_myForces.size(); i++) {
        std::stringstream f;
        f << _myParameterIndex << std::string(".forces[") << myCounter << "]";
        
        _myForces[i]->setShader(theParticles, theShader, f.str(), theWidth, theHeight);
        myCounter++;
    }
    
    setupParameter(0, 0);
}

void
GPUCombinedForce::setSize(int theWidth, int theHeight)
{
    for(unsigned int i=0; i<_myForces.size(); i++) {
        _myForces[i]->setSize(theWidth, theHeight);
    }
}

void
GPUCombinedForce::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    for(unsigned int i=0; i<_myForces.size(); i++) {
        _myForces[i]->update(theDeltaTime);
    }
}
