#ifndef __CCLIB_GPUPARTICLE_INCLUDED__
#define __CCLIB_GPUPARTICLE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec3.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <gl/gpuparticles.h>
#include <Exception.h>

namespace cclib {

class GPUParticle {
    private:
        double _myTimeOfDeath;
        float _myLifeTime;
        boolean _myIsAllocated;
        boolean _myIsPermanent;
	
        int _myStep;
        int _myIndex;
        Vector3f::Ptr _myPosition;
        Vector3f::Ptr  _myVelocity;
	
	    GPUParticles _myParticles;
	    float _myAge;

        GPUParticle(GPUParticles::Ptr theParticles, int theIndex);

    public: 
        typedef std::tr1::shared_ptr<GPUParticle> Ptr;
        Ptr create(GPUParticles::Ptr theParticles, int theIndex);
        virtual ~GPUParticle() {};

        void nextStep();
        void step(int theStep);
        int step();
        void age(float theAge);
        float age();
        bool isAllocated();
        void isAllocated(bool theIsAllocated);
        Vector3f::Ptr position();
	    Vector3f::Ptr velocity();

        bool isDead();
        float lifeTime();
        void lifeTime(float theLifeTime);
        bool isPermanent();
        void isPermanent(bool theIsPermanent);
        double timeOfDeath();
        void timeOfDeath(double theTimeOfDeath);
        int index();
        void index(int theIndex);
        int x();
        int y();
}
    
}; // namespace

#endif // includeguard



