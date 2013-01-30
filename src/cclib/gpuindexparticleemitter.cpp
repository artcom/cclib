
#include "gpuindexparticleemitter.h"

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

void 
ParticleWaitingList::add(GPUParticlePtr theParticle) {
    int myStep = (int)(theParticle->lifeTime() / _myTimeStep);
    myStep += _myOffset;
    myStep %= _myWaitLists->length;

    // if(_myWaitLists[myStep].empty()) {
    //     _myWaitLists[myStep] = new std::vector<CCGPUParticlePtr>();
    // }

    _myWaitLists[myStep].push_back(theParticle);
}
    
void 
ParticleWaitingList::handleCurrentWaitList(float theDeltaTime) {
    // if (_myCurrentWaitList == null)
    //     return;
    if (_myCurrentWaitList.empty()) {
        return;
    }

    float myFramesPerStep = _myTimeStep / theDeltaTime;
    int myChecksPerFrame = ceil(_myCurrentWaitList.size() / myFramesPerStep);

    for(int i = 0; i < myChecksPerFrame && _myCurrentWorkedIndex < _myCurrentWaitList.size(); i++, _myCurrentWorkedIndex++) {
        GPUParticle myParticle = _myCurrentWaitList[_myCurrentWorkedIndex];

        if(myParticle->isPermanent()) {
            _myPendingParticles->push_back(myParticle);
        } else {
            _myFreeIndices->push_back(myParticle->index());
        }
    }
}
    
void 
ParticleWaitingList::update(float theDeltaTime) {
    _myStepTime += theDeltaTime;

    handleCurrentWaitList(theDeltaTime);

    if(_myStepTime > _myTimeStep) {
        _myStepTime -= _myTimeStep;
        if(_myCurrentWaitList != null){
            for(;_myCurrentWorkedIndex < _myCurrentWaitList.size(); _myCurrentWorkedIndex++){
                GPUParticle myParticle = _myCurrentWaitList.get(_myCurrentWorkedIndex);

                if(myParticle->isPermanent()) {
                    _myPendingParticles.push_back(myParticle);
                } else {
                    _myFreeIndices.push_back(myParticle->index());
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
    
std::vector<GPUParticlePtr> 
ParticleWaitingList::deadParticles() {
    return _myDeadParticles;
}
    
void 
ParticleWaitingList::reset() {
    _myDeadParticles.clear();
    
    for(int i = 0; i < _myWaitLists.size(); i++) {
        _myWaitLists[i].clear();
    }
}



