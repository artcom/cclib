#ifndef __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__
#define __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__

#include <cclib.h>
#include <particles/gpuparticleemitter.h>
#include <capi/particleswrapper.h>

namespace cclib {

class ParticleWaitingList {

    private:
        float _myTimeStep;
        int _myOffset;
        float _myStepTime;
        int _myCurrentWorkedIndex;
        std::vector< std::vector<GPUParticlePtr> > _myWaitLists;
        ParticleWaitingList(float theTimeStep);

        unsigned int _myCurrentIdx;

    public:
        static ParticleWaitingListPtr create(float theTimeStep);
        void add(GPUParticlePtr theParticle);
        void update(float theDeltaTime, GPUIndexParticleEmitter * thePE);

        // std::vector<GPUParticlePtr> deadParticles();
        // void reset();

    private:
        void handleCurrentWaitList(float theDeltaTime, GPUIndexParticleEmitter * thePE);
};


class GPUIndexParticleEmitter : public GPUParticleEmitter, public Component {

    public:
        GPUParticlesPtr _myParticles;

    private:
        int _myNumberOfParticles;

    protected:
	    MeshPtr _myEmitMesh;
        double _myCurrentTime; // = 0;
    
    private:
        BufferPtr _myVertexBuffer;
	    BufferPtr _myColorBuffer;
	    BufferPtr _myPositionBuffer;
	    BufferPtr _myInfoBuffer;
	    BufferPtr _myVelocityBuffer;
        int _myStart;

    public:
        GPUIndexParticleEmitter() : Component("emitter") {};
        GPUIndexParticleEmitter(GPUParticlesPtr theParticles, int theStart, int theNumberParticles);

        virtual ~GPUIndexParticleEmitter() {}
        static GPUIndexParticleEmitterPtr create(GPUParticlesPtr theParticles, int theStart=0, int theNumberParticles=-1);
        // std::vector<GPUParticlePtr> & pendingParticles();
//        std::vector<GPUParticlePtr> & stateChangedParticles();
//        std::vector<GPUParticlePtr> & allocatedParticles();
//        int particlesInUse();
//        int freeParticles();
//        int start();
//        int numberOfParticles();
        int nextFreeId();
//        int xforIndex(int theIndex);
//        int yforIndex(int theIndex);
        void changeParticle(GPUParticlePtr theParticle);
        GPUParticlePtr emit(const Vector3f & thePosition, const Vector3f & theVelocity,
                float theLifeTime, bool theIsPermanent);
        GPUParticlePtr emit(const Vector3f & thePosition, const Vector3f & theVelocity, float theLifeTime);
        GPUParticlePtr emit(const Color & theColor, const Vector3f & thePosition, const Vector3f & theVelocity, float theLifeTime, bool theIsPermanent);

        GPUParticlePtr emit(int theIndex, const Color & theColor, const Vector3f & thePosition, const Vector3f & theVelocity,
                float theLifeTime, bool theIsPermanent);

        virtual void update(float theDeltaTime);
        void reset(); 
        int size();
        GPUParticlePtr particle(int theID);
        virtual void fillPositionData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles);
        virtual void fillColorData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles);
        virtual void fillInfoData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles);
        virtual void fillVelocityData(cclib::BufferPtr theBuffer, std::vector<cclib::GPUParticlePtr> & theParticles);
        virtual void transferEmitData();
        void transferInfoData();
        void transferColorData();
        void setData();
        void transferEmits();
        void transferChanges();

        // added due to the nested class private member access thing in the java original
        // std::vector<int> & freeIndices();
};
}; // namespace

#endif
