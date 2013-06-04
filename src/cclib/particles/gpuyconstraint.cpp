//
//  GPUYConstraint.cpp
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuyconstraint.h"
#include <particles/gpuplaneconstraint.h>

cclib::GPUYConstraint::GPUYConstraint(float theY, float theResilience, float theFriction, float theMinimalVelocity)
: cclib::GPUPlaneConstraint(
                            cclib::Plane3fPtr( new Plane3f(
                                                           cclib::Vector3fPtr( new cclib::Vector3f(0,theY,0) ),
                                                           cclib::Vector3fPtr( new cclib::Vector3f(0,1,0) )
                                                          
                                              )),
                            theResilience, theFriction, theMinimalVelocity
                            )
{
}

void cclib::GPUYConstraint::setY(float theY)
{
    _myPlane->setOriginNormal(
                              cclib::Vector3fPtr( new cclib::Vector3f(0,theY,0) ),
                              _myPlane->normal()
                              );
}
