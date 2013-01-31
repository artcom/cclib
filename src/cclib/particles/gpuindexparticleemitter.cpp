
#include <gl/mesh.h>
#include <gl/shader.h>
#include <gl/shadertexture.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuparticle.h>
#include <particles/gpuparticles.h>
#include "gpuindexparticleemitter.h"

using namespace cclib;

ParticleWaitingList::ParticleWaitingList(float theTimeStep) :
    _myOffset(0), 
    _myTimeStep(0),
    _myCurrentWorkedIndex(0),
    _myCurrentWaitList()
{
    _myTimeStep = theTimeStep;

    // asume a default max lifetime of 120 s
    int myNumberOfSteps = (int)(120 / theTimeStep);

    _myWaitLists = std::vector< std::vector<GPUParticlePtr> >( myNumberOfSteps, std::vector<GPUParticlePtr>() );
}
        
ParticleWaitingListPtr 
ParticleWaitingList::create(float theTimeStep) {
    return ParticleWaitingListPtr(new ParticleWaitingList(theTimeStep));
}

void 
ParticleWaitingList::add(GPUParticlePtr theParticle) {
    int myStep = (int)(theParticle->lifeTime() / _myTimeStep);
    myStep += _myOffset;
    myStep %= _myWaitLists.size();

    // if(_myWaitLists[myStep].empty()) {
    //     _myWaitLists[myStep] = new std::vector<CCGPUParticlePtr>();
    // }

    _myWaitLists[myStep].push_back(theParticle);
}
    
void 
ParticleWaitingList::handleCurrentWaitList(float theDeltaTime, GPUIndexParticleEmitterPtr thePE) {
    // if (_myCurrentWaitList == null)
    //     return;
    if (_myCurrentWaitList.empty()) {
        return;
    }

    float myFramesPerStep = _myTimeStep / theDeltaTime;
    int myChecksPerFrame = ceil(_myCurrentWaitList.size() / myFramesPerStep);

    for(int i = 0; i < myChecksPerFrame && _myCurrentWorkedIndex < _myCurrentWaitList.size(); i++, _myCurrentWorkedIndex++) {
        GPUParticlePtr myParticle = _myCurrentWaitList[_myCurrentWorkedIndex];

        if(myParticle->isPermanent()) {
            thePE->pendingParticles().push_back(myParticle);
        } else {
            thePE->freeIndices().push_back(myParticle->index());
        }
    }
}
    
void 
ParticleWaitingList::update(float theDeltaTime, GPUIndexParticleEmitterPtr thePE) {
    _myStepTime += theDeltaTime;

    handleCurrentWaitList(theDeltaTime, GPUIndexParticleEmitterPtr(this));

    if(_myStepTime > _myTimeStep) {
        _myStepTime -= _myTimeStep;
        if(_myCurrentWaitList.empty()) {
            for(;_myCurrentWorkedIndex < _myCurrentWaitList.size(); _myCurrentWorkedIndex++) {
                GPUParticlePtr myParticle = _myCurrentWaitList[_myCurrentWorkedIndex];

                if(myParticle->isPermanent()) {
                    thePE->pendingParticles().push_back(myParticle);
                } else {
                    thePE->freeIndices().push_back(myParticle->index());
                }
            }
            _myCurrentWorkedIndex = 0;
            _myCurrentWaitList.clear();
        }

        if(!_myWaitLists[_myOffset].empty()) {
            _myCurrentWaitList = _myWaitLists[_myOffset];
        }
        _myOffset++;
        _myOffset %= _myWaitLists.size();
    }
}
    
// std::vector<GPUParticlePtr> 
// ParticleWaitingList::deadParticles() {
//     return _myDeadParticles;
// }
//     
// void 
// ParticleWaitingList::reset() {
//     _myDeadParticles.clear();
//     
//     for(int i = 0; i < _myWaitLists.size(); i++) {
//         _myWaitLists[i].clear();
//     }
// }



	
GPUIndexParticleEmitter::GPUIndexParticleEmitter(GPUParticlesPtr theParticles, int theStart, int theNumberParticles) 
{
    if (theNumberParticles == -1) {
        theNumberParticles = theParticles->size();
    }

    _myParticles = theParticles;

    _myStart = theStart;

    _myNumberOfParticles = theNumberParticles;
    _myFreeIndices = std::vector<int>(_myNumberOfParticles, 0); 

    _myParticleWaitingList = ParticleWaitingList::create(0.5f);
    _myActiveParticlesArray = std::vector<GPUParticlePtr>();
    for(int i = 0; i < _myNumberOfParticles; i++) {
        int myIndex = _myStart + i;
        _myActiveParticlesArray[i] = GPUParticle::create(_myParticles, myIndex);
        _myFreeIndices.push_back(myIndex);
    }

    _myEmitMesh = Mesh::create(GL_POINTS);
    _myVertexBuffer = std::vector<float>(1000*3, 0.0f); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myPositionBuffer = std::vector<float>(1000*3, 0.0f); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myInfoBuffer = std::vector<float>(1000*3, 0.0f); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myVelocityBuffer = std::vector<float>(1000*3, 0.0f); // BufferUtil.newDirectFloatBuffer(1000 * 3);
}

GPUIndexParticleEmitterPtr 
GPUIndexParticleEmitter::create(GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
    return GPUIndexParticleEmitterPtr(new GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles));
}

std::vector<GPUParticlePtr> & 
GPUIndexParticleEmitter::pendingParticles() {
    return _myPendingParticles;
}

std::vector<GPUParticlePtr> & 
GPUIndexParticleEmitter::stateChangedParticles() {
    return _myStateChanges;
}

std::vector<GPUParticlePtr> & 
GPUIndexParticleEmitter::allocatedParticles() {
    return _myAllocatedParticles;
}

int 
GPUIndexParticleEmitter::particlesInUse(){
    return size() - _myFreeIndices.size();
}

int 
GPUIndexParticleEmitter::freeParticles() {
    return _myFreeIndices.size();
}

int 
GPUIndexParticleEmitter::start() {
    return _myStart;
}

int 
GPUIndexParticleEmitter::numberOfParticles() {
    return _myNumberOfParticles;
}

int 
GPUIndexParticleEmitter::nextFreeId() {
    if (_myFreeIndices.empty()) {
        return -1;
    }

    return _myFreeIndices.back(); 
}

std::vector<int> & 
GPUIndexParticleEmitter::freeIndices() {
    return _myFreeIndices; 
}

int 
GPUIndexParticleEmitter::xforIndex(int theIndex) {
    return (_myStart + theIndex) % _myParticles->width();
}

int 
GPUIndexParticleEmitter::yforIndex(int theIndex) {
    return (_myStart + theIndex) / _myParticles->width();
}

void 
GPUIndexParticleEmitter::changeParticle(GPUParticlePtr theParticle) {
    _myParticleWaitingList->add(theParticle);
    _myStateChanges.push_back(theParticle);
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(Vector3fPtr thePosition, Vector3fPtr theVelocity, 
        float theLifeTime, bool theIsPermanent)
{
    if(_myFreeIndices.empty()) {
        return GPUParticlePtr();
    }

    int myFreeIndex = nextFreeId(); 
    _myFreeIndices.pop_back();

    return emit(myFreeIndex, thePosition, theVelocity, theLifeTime, theIsPermanent);
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(int theIndex, Vector3fPtr thePosition, Vector3fPtr theVelocity, 
        float theLifeTime, bool theIsPermanent)
{
    int myIndex = theIndex - _myStart;
    GPUParticlePtr myActiveParticle = _myActiveParticlesArray[myIndex];
    myActiveParticle->position()->set( *(thePosition.get()) );
    myActiveParticle->velocity()->set( *(theVelocity.get()) );
    myActiveParticle->timeOfDeath(_myCurrentTime + theLifeTime);
    myActiveParticle->lifeTime(theLifeTime);
    myActiveParticle->isPermanent(theIsPermanent);
    myActiveParticle->step(0);

    _myAllocatedParticles.push_back(myActiveParticle);
    _myParticleWaitingList->add(myActiveParticle);

    return myActiveParticle;
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(Vector3fPtr thePosition, Vector3fPtr theVelocity, float theLifeTime) {
    return emit(thePosition, theVelocity, theLifeTime, false);
}

void 
GPUIndexParticleEmitter::update(float theDeltaTime) {
    _myParticleWaitingList->update(theDeltaTime, GPUIndexParticleEmitterPtr(this));
}

int 
GPUIndexParticleEmitter::size() {
    return _myNumberOfParticles;
}

GPUParticlePtr 
GPUIndexParticleEmitter::particle(int theID) {
    return _myActiveParticlesArray[theID - _myStart];
}

void GPUIndexParticleEmitter::fillPositionData(std::vector<float> & theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer[i * 3 + 0] = myParticle->position()->x();
        theBuffer[i * 3 + 1] = myParticle->position()->y();
        theBuffer[i * 3 + 2] = myParticle->position()->z();
    }
}

void 
GPUIndexParticleEmitter::fillInfoData(std::vector<float> & theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer[i * 3 + 0] = 0;
        theBuffer[i * 3 + 1] = myParticle->lifeTime();
        theBuffer[i * 3 + 2] = myParticle->isPermanent() ? 1 : 0;//, myParticle.step();
    }
}

void 
GPUIndexParticleEmitter::fillVelocityData(std::vector<float> & theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer[i * 3 + 0] = myParticle->velocity()->x();
        theBuffer[i * 3 + 1] = myParticle->velocity()->y();
        theBuffer[i * 3 + 2] = myParticle->velocity()->z();
    }
}

void 
GPUIndexParticleEmitter::transferData() {
    _myParticles->dataTexture()->beginDraw();
    _myParticles->initValueShader()->start();

    _myEmitMesh->draw();

    _myParticles->initValueShader()->end();
    _myParticles->dataTexture()->endDraw();
}

void 
GPUIndexParticleEmitter::setData() {

    int myEmitSize = _myAllocatedParticles.size();
    if (myEmitSize == 0) {
        return;
    }

    // XXX unsure if the works. the structures were Java List<foo> using .limit for resizing and .put to set the data
    //push_back and clear might not be equivalent in that case.

    // if (myEmitSize > _myEmitMesh.numberOfVertices()) {
    //     _myVertexBuffer = std::vector<float>(myEmitSize * 3, 0.0f);
    //     _myPositionBuffer = std::vector<float>(myEmitSize * 3, 0.0f);
    //     _myInfoBuffer = std::vector<float>(myEmitSize * 3, 0.0f);
    //     _myVelocityBuffer = std::vector<float>(myEmitSize * 3, 0.0f);
    // } else {
    //     _myVertexBuffer.limit(myEmitSize * 3);
    //     _myPositionBuffer.limit(myEmitSize * 3);
    //     _myInfoBuffer.limit(myEmitSize * 3);
    //     _myVelocityBuffer.limit(myEmitSize * 3);
    // }
    _myVertexBuffer.clear();
    _myPositionBuffer.clear();
    _myInfoBuffer.clear();
    _myVelocityBuffer.clear();

    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        _myVertexBuffer.push_back(myParticle->x() + 0.5f);
        _myVertexBuffer.push_back(myParticle->y() + 0.5f);
        _myVertexBuffer.push_back(0.0f);
    }

    fillPositionData(_myPositionBuffer);
    fillInfoData(_myInfoBuffer);
    fillVelocityData(_myVelocityBuffer);

    _myEmitMesh->clearAll();
    _myEmitMesh->vertices(_myVertexBuffer);
    _myEmitMesh->textureCoords(0, _myPositionBuffer, 3);
    _myEmitMesh->textureCoords(1, _myInfoBuffer, 3);
    _myEmitMesh->textureCoords(2, _myVelocityBuffer, 3);

    transferData(theGraphics);

    _myAllocatedParticles.clear();
}

