
#include "gpuparticle.h"
#include <particles/gpuparticles.h>

using namespace cclib;

GPUParticle::GPUParticle(GPUParticlesPtr theParticles, int theIndex):
    _myParticles(theParticles), 
    _myIndex(theIndex), 
    _myIsAllocated(false), 
    _myStep(0), 
    _myAge(0), 
    _myIsPermanent(false)
{
    _myPosition = Vector3fPtr( new Vector3f() );
    _myVelocity = Vector3fPtr( new Vector3f() );
    _myColor = Color::Ptr( new Color() );
}


GPUParticlePtr
GPUParticle::create(GPUParticlesPtr theParticles, int theIndex) {
    return GPUParticlePtr(new GPUParticle(theParticles, theIndex));
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
GPUParticle::isAllocated(bool theIsAllocated) {
    _myIsAllocated = theIsAllocated;
}

ColorPtr
GPUParticle::color() {
    return _myColor;
}

Vector3fPtr 
GPUParticle::position() {
    return _myPosition;
}

Vector3fPtr 
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
GPUParticle::isPermanent(bool theIsPermanent) {
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




