
#include "gpuviscousdrag.h"

using namespace cclib; 

GPUViscousDrag::GPUViscousDrag(float theCoefficient) :
    GPUForce("ViscousDrag"), 
    _myCoefficient(Property_<float>::create("coefficient", theCoefficient)) 
{
    registerProperty(_myCoefficient);
}

GPUViscousDragPtr
GPUViscousDrag::create(float theCoefficient) {
    return GPUViscousDragPtr(new GPUViscousDrag(theCoefficient));
}

void 
GPUViscousDrag::setDrag(float theDrag) {
    _myCoefficient->setValue<float>(theDrag);
}

float 
GPUViscousDrag::getDrag() {
    return _myCoefficient->getValue<float>();
}

void 
GPUViscousDrag::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);
    _myCoefficientParameter = parameter("coefficient");
}

void 
GPUViscousDrag::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);
    _myVelocityShader->parameter(_myCoefficientParameter, _myCoefficient);
}
