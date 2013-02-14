
#include "gpuviscousdrag.h"

using namespace cclib; 

GPUViscousDrag::GPUViscousDrag(float theCoefficient) :
            GPUForce("ViscousDrag"), 
            _myCoefficient(theCoefficient) 
{}

GPUViscousDragPtr
GPUViscousDrag::create(float theCoefficient) {
    return GPUViscousDragPtr(new GPUViscousDrag(theCoefficient));
}

void 
GPUViscousDrag::drag(float theDrag) {
    _myCoefficient = theDrag;
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
