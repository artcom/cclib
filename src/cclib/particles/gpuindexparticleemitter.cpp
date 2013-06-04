
#include <gl/mesh.h>
#include <gl/cgshader.h>
#include <gl/shaderbuffer.h>
#include <gl/bufferobject.h>
#include <gl/graphics.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuparticle.h>
#include <particles/gpuparticles.h>
#include "gpuindexparticleemitter.h"

using namespace cclib;

ParticleWaitingList::ParticleWaitingList(float theTimeStep) :
    _myTimeStep(0),
    _myOffset(0),
    _myCurrentWorkedIndex(0),
    _myCurrentIdx(0)
{
    _myTimeStep = theTimeStep;

    // asume a default max lifetime of 120 s
    int myNumberOfSteps = (int)(12 / theTimeStep);

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
ParticleWaitingList::handleCurrentWaitList(float theDeltaTime, GPUIndexParticleEmitter * thePE) {
    if (_myWaitLists[_myCurrentIdx].empty()) {
        return;
    }

    float myFramesPerStep = _myTimeStep / theDeltaTime;
    int myChecksPerFrame = ceil(_myWaitLists[_myCurrentIdx].size() / myFramesPerStep);

    for(int i = 0; i < myChecksPerFrame && _myCurrentWorkedIndex < _myWaitLists[_myCurrentIdx].size(); i++, _myCurrentWorkedIndex++) {
        GPUParticlePtr myParticle = _myWaitLists[_myCurrentIdx][_myCurrentWorkedIndex];

        if(myParticle->isPermanent()) {
            thePE->pendingParticles().push_back(myParticle);
        } else {
            thePE->freeIndices().push_back(myParticle->index());
        }
    }
}
    
void 
ParticleWaitingList::update(float theDeltaTime, GPUIndexParticleEmitter * thePE) {
    _myStepTime += theDeltaTime;

    handleCurrentWaitList(theDeltaTime, thePE);

    if(_myStepTime > _myTimeStep) {

        _myStepTime -= _myTimeStep;
        if(!_myWaitLists[_myCurrentIdx].empty()) {
            for(;_myCurrentWorkedIndex < _myWaitLists[_myCurrentIdx].size(); _myCurrentWorkedIndex++) {
                GPUParticlePtr myParticle = _myWaitLists[_myCurrentIdx][_myCurrentWorkedIndex];

                if(myParticle->isPermanent()) {
                    thePE->pendingParticles().push_back(myParticle);
                } else {
                    thePE->freeIndices().push_back(myParticle->index());
                }
            }
            _myCurrentWorkedIndex = 0;
            _myWaitLists[_myCurrentIdx] = std::vector<GPUParticlePtr>();
        }

        if(!_myWaitLists[_myOffset].empty()) {
            _myCurrentIdx = _myOffset;
        }
        _myOffset++;
        _myOffset %= _myWaitLists.size();
    }
    
    // std::cout << "> \t";
    // for (unsigned int i=0; i<_myWaitLists.size(); i++) {
    //     std::cout << _myWaitLists[i].size() << "\t";
    // }
    // std::cout << std::endl;
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



	
GPUIndexParticleEmitter::GPUIndexParticleEmitter(GPUParticlesPtr theParticles, int theStart, int theNumberParticles) :
    _myCurrentTime(0)
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
        _myActiveParticlesArray.push_back(GPUParticle::create(_myParticles, myIndex));
        _myFreeIndices[i] = myIndex;
    }

    _myEmitMesh = Mesh::create(GL_POINTS);
    _myVertexBuffer = Buffer::create(1000*3); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myPositionBuffer = Buffer::create(1000*3); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myInfoBuffer = Buffer::create(1000*4); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myVelocityBuffer = Buffer::create(1000*3); // BufferUtil.newDirectFloatBuffer(1000 * 3);
    _myColorBuffer = Buffer::create(1000*4);
}

GPUIndexParticleEmitterPtr 
GPUIndexParticleEmitter::create(GPUParticlesPtr theParticles, int theStart, int theNumberParticles) {
    return GPUIndexParticleEmitterPtr(new GPUIndexParticleEmitter(theParticles, theStart, theNumberParticles));
}

std::vector<GPUParticlePtr> & 
GPUIndexParticleEmitter::pendingParticles() {
    return _myPendingParticles;
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

void 
GPUIndexParticleEmitter::changeParticle(GPUParticlePtr theParticle) {
    _myParticleWaitingList->add(theParticle);
    _myStateChanges.push_back(theParticle);
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(const Color & theColor, const Vector3f & thePosition, const Vector3f & theVelocity, float theLifeTime, bool theIsPermanent) {
    if (_myFreeIndices.empty()) {
        return GPUParticlePtr();
    }

    int myFreeIndex = nextFreeId();
    _myFreeIndices.pop_back();
    return emit(myFreeIndex, theColor, thePosition, theVelocity, theLifeTime, theIsPermanent);
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(const Vector3f & thePosition, const Vector3f & theVelocity,
        float theLifeTime, bool theIsPermanent)
{
    if(_myFreeIndices.empty()) {
        return GPUParticlePtr();
    }

    int myFreeIndex = nextFreeId(); 
    _myFreeIndices.pop_back();
    Color myColor(1.0f, 1.0f, 1.0f, 1.0);
    return emit(myFreeIndex, myColor, thePosition, theVelocity, theLifeTime, theIsPermanent);
}

GPUParticlePtr 
GPUIndexParticleEmitter::emit(int theIndex, const Color & theColor, const Vector3f & thePosition, const Vector3f & theVelocity,
        float theLifeTime, bool theIsPermanent)
{
    int myIndex = theIndex - _myStart;
    GPUParticlePtr myActiveParticle = _myActiveParticlesArray[myIndex];
    myActiveParticle->color()->set( theColor );
    myActiveParticle->position()->set( thePosition );
    myActiveParticle->velocity()->set( theVelocity );
    myActiveParticle->timeOfDeath(_myCurrentTime + theLifeTime);
    myActiveParticle->lifeTime(theLifeTime);
    myActiveParticle->isPermanent(theIsPermanent);
    myActiveParticle->step(0);
    
    _myAllocatedParticles.push_back(myActiveParticle);
    _myParticleWaitingList->add(myActiveParticle);

    return myActiveParticle;
}

GPUParticlePtr
GPUIndexParticleEmitter::emit(const Vector3f & thePosition, const Vector3f & theVelocity, float theLifeTime) {
    return emit(thePosition, theVelocity, theLifeTime, false);
}

void 
GPUIndexParticleEmitter::update(float theDeltaTime) {
    _myParticleWaitingList->update(theDeltaTime, this);
}

int
GPUIndexParticleEmitter::size() {
    return _myNumberOfParticles;
}

GPUParticlePtr 
GPUIndexParticleEmitter::particle(int theID) {
    return _myActiveParticlesArray[theID - _myStart];
}

void GPUIndexParticleEmitter::fillPositionData(BufferPtr theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer->data()[i * 3 + 0] = myParticle->position()->x();
        theBuffer->data()[i * 3 + 1] = myParticle->position()->y();
        theBuffer->data()[i * 3 + 2] = myParticle->position()->z();
    }
}

void GPUIndexParticleEmitter::fillColorData(BufferPtr theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer->data()[i * 4 + 0] = myParticle->color()->red();
        theBuffer->data()[i * 4 + 1] = myParticle->color()->green();
        theBuffer->data()[i * 4 + 2] = myParticle->color()->blue();
        theBuffer->data()[i * 4 + 3] = myParticle->color()->alpha();
    }
}

void 
GPUIndexParticleEmitter::fillInfoData(BufferPtr theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer->data()[i * 4 + 0] = myParticle->age();
        theBuffer->data()[i * 4 + 1] = myParticle->lifeTime();
        theBuffer->data()[i * 4 + 2] = 1;//myParticle->isPermanent() ? 1 : 0;//, myParticle.step();
        theBuffer->data()[i * 4 + 3] = myParticle->step(); //, myParticle.step();
    }
}

void 
GPUIndexParticleEmitter::fillVelocityData(BufferPtr theBuffer) {
    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        theBuffer->data()[i * 3 + 0] = myParticle->velocity()->x();
        theBuffer->data()[i * 3 + 1] = myParticle->velocity()->y();
        theBuffer->data()[i * 3 + 2] = myParticle->velocity()->z();
    }
}

void 
GPUIndexParticleEmitter::transferData() {
    
    //printf("%s\n",__PRETTY_FUNCTION__);
    
    _myParticles->dataBuffer()->beginDraw();
    Graphics::checkError();
    
    _myParticles->initValueShader()->start();
    Graphics::checkError();

    _myEmitMesh->draw();
    Graphics::checkError();

    _myParticles->initValueShader()->end();
    Graphics::checkError();

    _myParticles->dataBuffer()->endDraw();
    Graphics::checkError();
    
//    printf("%s END END END END END END END END END END\n",__PRETTY_FUNCTION__);
}

void 
GPUIndexParticleEmitter::setData() {
    
    //printf("%s\n",__PRETTY_FUNCTION__);
    
    int myEmitSize = _myAllocatedParticles.size();
    if (myEmitSize == 0) {
        return;
    }

    if (myEmitSize > _myEmitMesh->numberOfVertices()) {
        _myVertexBuffer = Buffer::create(myEmitSize * 3, true);
        _myPositionBuffer = Buffer::create(myEmitSize * 3, true);
        _myInfoBuffer = Buffer::create(myEmitSize * 4, true);
        _myVelocityBuffer = Buffer::create(myEmitSize * 3, true);
        _myColorBuffer = Buffer::create(myEmitSize * 4, true);
    }
#warning implement correct resizing
    /* else {
        _myVertexBuffer.limit(myEmitSize * 3);
        _myPositionBuffer.limit(myEmitSize * 3);
        _myInfoBuffer.limit(myEmitSize * 3);
        _myVelocityBuffer.limit(myEmitSize * 3);
    } */
    
    _myVertexBuffer->rewind();
    _myColorBuffer->rewind();
    _myPositionBuffer->rewind();
    _myInfoBuffer->rewind();
    _myVelocityBuffer->rewind();

    for (unsigned int i=0; i<_myAllocatedParticles.size(); i++) {
        GPUParticlePtr myParticle = _myAllocatedParticles[i];
        _myVertexBuffer->put(myParticle->x() + 0.5f);
        _myVertexBuffer->put(myParticle->y() + 0.5f);
        _myVertexBuffer->put(0.0f);
    }
    
    fillPositionData(_myPositionBuffer);
    fillColorData(_myColorBuffer);
    fillInfoData(_myInfoBuffer);
    fillVelocityData(_myVelocityBuffer);

    _myVertexBuffer->rewind();
    _myColorBuffer->rewind();
    _myPositionBuffer->rewind();
    _myInfoBuffer->rewind();
    _myVelocityBuffer->rewind();

    _myEmitMesh->clearAll();
    _myEmitMesh->vertices(_myVertexBuffer);
    _myEmitMesh->textureCoords(0, _myPositionBuffer, 3);
    _myEmitMesh->textureCoords(1, _myInfoBuffer, 4);
    _myEmitMesh->textureCoords(2, _myVelocityBuffer, 3);
    _myEmitMesh->textureCoords(3, _myColorBuffer, 4);

    transferData();

    _myAllocatedParticles.clear();
}

//std::vector<GPUParticlePtr> &
//GPUIndexParticleEmitter::stateChangedParticles() {
//    return _myStateChanges;
//}
//
//std::vector<GPUParticlePtr> &
//GPUIndexParticleEmitter::allocatedParticles() {
//    return _myAllocatedParticles;
//}
//
//int
//GPUIndexParticleEmitter::particlesInUse(){
//    return size() - _myFreeIndices.size();
//}
//
//int
//GPUIndexParticleEmitter::freeParticles() {
//    return _myFreeIndices.size();
//}
//
//int
//GPUIndexParticleEmitter::start() {
//    return _myStart;
//}
//
//int
//GPUIndexParticleEmitter::numberOfParticles() {
//    return _myNumberOfParticles;
//}
//int
//GPUIndexParticleEmitter::xforIndex(int theIndex) {
//    return (_myStart + theIndex) % _myParticles->width();
//}
//
//int
//GPUIndexParticleEmitter::yforIndex(int theIndex) {
//    return (_myStart + theIndex) / _myParticles->width();
//}



