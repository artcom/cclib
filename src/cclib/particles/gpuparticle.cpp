
#include <gl/gpuparticle.h>

using namespace cclib;

GPUParticle::GPUParticle(GPUParticles::Ptr theParticles, int theIndex):
    _myParticles(theParticles), _myIndex(theIndex), _myIsAllocated(false), 
    _myStep(0), _myAge(0), _myIsPermanent(false)
{
    _myPosition = Ptr(new Vector3f());
    _myVelocity = Ptr(new Vector3f());
}

GPUParticle::Ptr 
GPUParticle::create(GPUParticles::Ptr theParticles, int theIndex) {
    return Ptr(new GPUParticle(theParticles, theIndex));
}

void 
GPUParticle::nextStep() {
    _myStep++;
}

void 
GPUParticle::step(int theStep) {
    _myStep = theStep;
}

int 
GPUParticle::step() {
    return _myStep;
}

void 
GPUParticle::age(float theAge) {
    _myAge = theAge;
}

float 
GPUParticle::age() {
    return _myAge;
}

bool 
GPUParticle::isAllocated() {
    return _myIsAllocated;
}

void 
GPUParticle::isAllocated(boolean theIsAllocated) {
    _myIsAllocated = theIsAllocated;
}

Vector3f::Ptr 
GPUParticle::position() {
    return _myPosition;
}

Vector3f::Ptr 
GPUParticle::velocity() {
    return _myVelocity;
}

// XXX  do something c++-y
// public int compareTo(GPUParticle theParticle) {
//     if(_myTimeOfDeath < theParticle._myTimeOfDeath)return -1;
//     return 1;
// }

bool 
GPUParticle::isDead() {
    return _myTimeOfDeath < _myParticles->currentTime();
}

float 
GPUParticle::lifeTime() {
    return _myLifeTime;
}

void 
GPUParticle::lifeTime(float theLifeTime) {
    _myLifeTime = theLifeTime;
}

bool 
GPUParticle::isPermanent() {
    return _myIsPermanent;
}

void 
GPUParticle::isPermanent(boolean theIsPermanent) {
    _myIsPermanent = theIsPermanent;
}

double 
GPUParticle::timeOfDeath() {
    return _myTimeOfDeath;
}

void 
GPUParticle::timeOfDeath(double theTimeOfDeath) {
    _myTimeOfDeath = theTimeOfDeath;
}

int 
GPUParticle::index() {
    return _myIndex;
}

void 
GPUParticle::index(int theIndex) {
    _myIndex = theIndex;
}

int 
GPUParticle::x() {
    return _myIndex % _myParticles->width();
}

int 
GPUParticle::y() {
    return _myIndex / _myParticles->width();
}




