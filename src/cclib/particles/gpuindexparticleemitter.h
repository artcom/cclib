#ifndef __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__
#define __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__

#include <cclib.h>
#include <particles/gpuparticleemitter.h>

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


class GPUIndexParticleEmitter : public GPUParticleEmitter {

    private:
        int _myNumberOfParticles;
	
    protected:
        GPUParticlesPtr _myParticles;
        std::vector<GPUParticlePtr> _myActiveParticlesArray;
	    MeshPtr _myEmitMesh;
        double _myCurrentTime; // = 0;
        std::vector<GPUParticlePtr> _myAllocatedParticles; // = new ArrayList<GPUParticle>();
    
    private:
	    std::vector<GPUParticlePtr> _myDeadParticles; // = new ArrayList<GPUParticle>();
	    std::vector<GPUParticlePtr> _myPendingParticles; // = new ArrayList<GPUParticle>();
	    std::vector<GPUParticlePtr> _myStateChanges; // = new ArrayList<GPUParticle>();
	    std::vector<int> _myFreeIndices;
	
        ParticleWaitingListPtr _myParticleWaitingList;
        
        BufferPtr _myVertexBuffer;
	    BufferPtr _myColorBuffer;
	    BufferPtr _myPositionBuffer;
	    BufferPtr _myInfoBuffer;
	    BufferPtr _myVelocityBuffer;
        int _myStart;
	
    public:
        GPUIndexParticleEmitter() {};
        GPUIndexParticleEmitter(GPUParticlesPtr theParticles, int theStart, int theNumberParticles);
    
        ~GPUIndexParticleEmitter() {
            std::cout << "hueh??" << std::endl;
        }
        static GPUIndexParticleEmitterPtr create(GPUParticlesPtr theParticles, int theStart=0, int theNumberParticles=-1);
        std::vector<GPUParticlePtr> & pendingParticles();
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
        
        void update(float theDeltaTime);
        int size();
        GPUParticlePtr particle(int theID);
        virtual void fillPositionData(BufferPtr theBuffer);
        void fillColorData(BufferPtr theBuffer);
        void fillInfoData(BufferPtr theBuffer);
        void fillVelocityData(BufferPtr theBuffer);
        virtual void transferData();
        void setData();

        // added due to the nested class private member access thing in the java original
        std::vector<int> & freeIndices();
};
}; // namespace

#endif
