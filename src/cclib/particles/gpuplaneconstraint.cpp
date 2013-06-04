//
//  GPUPlaneConstraint.cpp
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuplaneconstraint.h"
#include <particles/gpuupdateshader.h>

cclib::GPUPlaneConstraint::GPUPlaneConstraint(cclib::Plane3fPtr thePlane, float theResilience, float theFriction, float theMinimalVelocity)
: cclib::GPUConstraint("PlaneConstraint", theResilience, theFriction, theMinimalVelocity)
{
    _myPlane = thePlane;
}

void cclib::GPUPlaneConstraint::setupParameter(int theWidth, int theHeight)
{
    _myConstantParameter = parameter("constant");
    _myNormalParameter = parameter("normal");
}

void cclib::GPUPlaneConstraint::update(float theDeltaTime)
{
    _myVelocityShader->parameter(_myConstantParameter, _myPlane->constant());
    _myVelocityShader->parameter(_myNormalParameter, _myPlane->normal());
}