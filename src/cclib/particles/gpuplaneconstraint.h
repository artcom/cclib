//
//  GPUPlaneConstraint.h
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#ifndef __Project__GPUPlaneConstraint__
#define __Project__GPUPlaneConstraint__

#include <cclib.h>
#include <particles/gpuconstraint.h>

namespace cclib {
    
    class GPUPlaneConstraint : public cclib::GPUConstraint
    {
        CGparameter _myConstantParameter;
        CGparameter _myNormalParameter;
        
    protected:
        cclib::Plane3fPtr _myPlane;

    public:
        
        GPUPlaneConstraint(cclib::Plane3fPtr thePlane, float theResilience, float theFriction, float theMinimalVelocity);
        ~GPUPlaneConstraint() {};
        
        static GPUPlaneConstraintPtr create(cclib::Plane3fPtr thePlane, float theResilience, float theFriction, float theMinimalVelocity)
        {
            return GPUPlaneConstraintPtr(new GPUPlaneConstraint(thePlane, theResilience, theFriction, theMinimalVelocity));
        }
        
        void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        
    private:
    };
    
}; // namespace cclib

#endif /* defined(__Project__GPUPlaneConstraint__) */
