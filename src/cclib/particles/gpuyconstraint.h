//
//  GPUYConstraint.h
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#ifndef __Project__GPUYConstraint__
#define __Project__GPUYConstraint__

#include <cclib.h>
#include <particles/gpuplaneconstraint.h>

namespace cclib {
    
    class GPUYConstraint;
    typedef std::tr1::shared_ptr<GPUYConstraint> GPUYConstraintPtr;
    
    class GPUYConstraint : public cclib::GPUPlaneConstraint
    {
    public:
        
        GPUYConstraint(float theY, float theResilience, float theFriction, float theMinimalVelocity);
        ~GPUYConstraint() {};
        
        static GPUYConstraintPtr create(float theY, float theResilience, float theFriction, float theMinimalVelocity)
        {
            return GPUYConstraintPtr(new GPUYConstraint(theY, theResilience, theFriction, theMinimalVelocity));
        }
        
        void setY(float theY);
        
    private:
    };
    
}; // namespace cclib

#endif /* defined(__Project__GPUYConstraint__) */
