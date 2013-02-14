
#ifndef __CCLIB_GPUVISCOUSDRAG_INCLUDED__
#define __CCLIB_GPUVISCOUSDRAG_INCLUDED__

#include <cclib.h>
#include <particles/gpuforce.h>

namespace cclib {

class GPUViscousDrag : public GPUForce {

    private:
        float _myCoefficient;
        CGparameter _myCoefficientParameter;
	
        GPUViscousDrag(float theCoefficient); 

    public:
        static GPUViscousDragPtr create(float theCoefficient);

	    void setupParameter(int theWidth, int theHeight);
        void update(float theDeltaTime);
        void drag(float theDrag);
};	
};

#endif 
