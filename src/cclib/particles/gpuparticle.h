#ifndef __CCLIB_GPUPARTICLE_INCLUDED__
#define __CCLIB_GPUPARTICLE_INCLUDED__

#include <cclib.h>

namespace cclib {

class GPUParticle {
    private:
        double _myTimeOfDeath;
        float _myLifeTime;
        bool _myIsAllocated;
        bool _myIsPermanent;
	
        int _myStep;
        int _myIndex;
        Vector3f::Ptr _myPosition;
        Vector3f::Ptr  _myVelocity;
	
        GPUParticlesPtr _myParticles;
	    float _myAge;

        GPUParticle(GPUParticlesPtr theParticles, int theIndex);

    public: 
        GPUParticlePtr create(GPUParticlesPtr theParticles, int theIndex);
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
};
    
}; // namespace

#endif // includeguard



