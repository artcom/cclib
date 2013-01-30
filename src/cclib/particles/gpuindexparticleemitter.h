#ifndef __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__
#define __CCLIB_GPUINDEXPARTICLEEMITTER_INCLUDED__

#include <cclib.h>

namespace cclib {
	
class ParticleWaitingList {
    
    private:
        float _myTimeStep;
        int _myOffset; //  = 0;
        float _myStepTime; 
        int _myCurrentWorkedIndex; 
        std::vector< std::vector<GPUParticlePtr> > _myWaitLists;
        std::vector<CCGPUParticlePtr> _myCurrentWaitList;

    public:
        ParticleWaitingList(float theTimeStep);
        void add(GPUParticlePtr theParticle);
        void update(float theDeltaTime);
        std::vector<GPUParticlePtr> deadParticles();
        void reset(); 
        
    private:
        void handleCurrentWaitList(float theDeltaTime);
}


class GPUIndexParticleEmitter : public CCGPUParticleEmitter {
	
	
	private int _myNumberOfParticles;
	
	protected CCGPUParticles _myParticles;
	
	private List<CCGPUParticle> _myAllocatedParticles = new ArrayList<CCGPUParticle>();
	private List<CCGPUParticle> _myDeadParticles = new ArrayList<CCGPUParticle>();
	private List<CCGPUParticle> _myPendingParticles = new ArrayList<CCGPUParticle>();
	private List<CCGPUParticle> _myStateChanges = new ArrayList<CCGPUParticle>();
	private List<Integer> _myFreeIndices;
	
	private CCParticleWaitingList _myParticleWaitingList;
	protected final CCGPUParticle[] _myActiveParticlesArray;
	
	protected CCMesh _myEmitMesh;
	private FloatBuffer _myVertexBuffer;
	private FloatBuffer _myPositionBuffer;
	private FloatBuffer _myInfoBuffer;
	private FloatBuffer _myVelocityBuffer;
	protected float[] _myFillArray;
	
	private int _myStart;
	
	protected double _myCurrentTime = 0;
	
	public CCGPUIndexParticleEmitter(CCGPUParticles theParticles, int theStart, int theNumberParticles) {
		_myParticles = theParticles;
		
		_myStart = theStart;
		
		_myNumberOfParticles = theNumberParticles;
		_myFreeIndices = new ArrayList<Integer>(_myNumberOfParticles);
		
		_myParticleWaitingList = new CCParticleWaitingList(0.5f);
		_myActiveParticlesArray = new CCGPUParticle[_myNumberOfParticles];
		for(int i = 0; i < _myActiveParticlesArray.length;i++) {
			int myIndex = _myStart + i;
			_myActiveParticlesArray[i] = new CCGPUParticle(_myParticles, myIndex);
			_myFreeIndices.add(myIndex);
		}
		
		_myEmitMesh = new CCMesh(CCDrawMode.POINTS);
		_myVertexBuffer = CCBufferUtil.newDirectFloatBuffer(1000 * 3);
		_myPositionBuffer = CCBufferUtil.newDirectFloatBuffer(1000 * 3);
		_myInfoBuffer = CCBufferUtil.newDirectFloatBuffer(1000 * 3);
		_myVelocityBuffer = CCBufferUtil.newDirectFloatBuffer(1000 * 3);
		_myFillArray = new float[1000 * 3];
	}
	
	public CCGPUIndexParticleEmitter(CCGPUParticles theParticles) {
		this(theParticles, 0, theParticles.size());
	}
	
	public List<CCGPUParticle> pendingParticles(){
		return _myPendingParticles;
	}
	
	public List<CCGPUParticle> stateChangedParticles(){
		return _myStateChanges;
	}
	
	public List<CCGPUParticle> allocatedParticles(){
		return _myAllocatedParticles;
	}
	
	/**
	 * Returns the number of currently active particles
	 * @return
	 */
	public int particlesInUse(){
		return size() - _myFreeIndices.size();
	}
	
	/**
	 * Returns the number of particles that can still be allocated from this particle system
	 * @return
	 */
	public int freeParticles() {
		return _myFreeIndices.size();
	}
	
	public int start() {
		return _myStart;
	}
	
	public int numberOfParticles() {
		return _myNumberOfParticles;
	}
	
	public int nextFreeId() {
		if (_myFreeIndices.isEmpty())
			return -1;

		return _myFreeIndices.get(_myFreeIndices.size() - 1);
	}
	
	public int xforIndex(int theIndex) {
		return (_myStart + theIndex) % _myParticles.width();
	}
	
	public int yforIndex(int theIndex) {
		return (_myStart + theIndex) / _myParticles.width();
	}
	
	public void changeParticle(CCGPUParticle theParticle) {
		_myParticleWaitingList.add(theParticle);
		_myStateChanges.add(theParticle);
	}

	/**
	 * Allocates a new particle with the given position, velocity and data.
	 * You can also define if a particle is permanent or can die. The number
	 * of particles you can create is limited by the size of the data texture
	 * that you define in the constructor of the particle system. If no particle
	 * could be allocated this method returns null.
	 * @param thePosition position of the particle
	 * @param theVelocity velocity of the particle
	 * @param theLifeTime lifetime of the particle
	 * @param theIsPermanent <code>true</code> if the particle is permanent otherwise<code>false</code>
	 * @return the allocated particle or <code>null</code>
	 */
	public CCGPUParticle emit(
		final CCVector3f thePosition, 
		final CCVector3f theVelocity, 
		final float theLifeTime, 
		final boolean theIsPermanent
	){
		if(_myFreeIndices.isEmpty())return null;

		int myFreeIndex = _myFreeIndices.remove(_myFreeIndices.size() - 1);
		
		return emit(myFreeIndex, thePosition, theVelocity, theLifeTime, theIsPermanent);
	}
	
	/**
	 * Allocates a new particle with the given position, velocity and data.
	 * You can also define if a particle is permanent or can die. The number
	 * of particles you can create is limited by the size of the data texture
	 * that you define in the constructor of the particle system. If no particle
	 * could be allocated this method returns null.
	 * @param thePosition position of the particle
	 * @param theVelocity velocity of the particle
	 * @param theLifeTime lifetime of the particle
	 * @param theIsPermanent <code>true</code> if the particle is permanent otherwise<code>false</code>
	 * @return the allocated particle or <code>null</code>
	 */
	public CCGPUParticle emit(
		final int theIndex,
		final CCVector3f thePosition, 
		final CCVector3f theVelocity, 
		final float theLifeTime, 
		final boolean theIsPermanent
	){
//		_myAvailableIndices.remove(theIndex);
		int myIndex = theIndex - _myStart;
		CCGPUParticle myActiveParticle = _myActiveParticlesArray[myIndex];
		myActiveParticle.position().set(thePosition);
		myActiveParticle.velocity().set(theVelocity);
		myActiveParticle.timeOfDeath(_myCurrentTime + theLifeTime);
		myActiveParticle.lifeTime(theLifeTime);
		myActiveParticle.isPermanent(theIsPermanent);
		myActiveParticle.step(0);

		_myAllocatedParticles.add(myActiveParticle);
		_myParticleWaitingList.add(myActiveParticle);
		
		return myActiveParticle;
	}
	
	/**
	 * Allocates a new particle with the given position, velocity and data.
	 * The number of particles you can create is limited by the size of the data texture
	 * that you define in the constructor of the particle system. If no particle
	 * could be allocated this method returns null.
	 * @param thePosition position of the particle
	 * @param theVelocity velocity of the particle
	 * @param theLifeTime lifetime of the particle
	 * @return the allocated particle or <code>null</code>
	 */
	public CCGPUParticle emit(
		final CCVector3f thePosition, 
		final CCVector3f theVelocity, 
		final float theLifeTime
	) {
		return emit(thePosition, theVelocity, theLifeTime, false);
	}
	
//	public void kill() {
//		for(CCGPUParticle myParticle:_myAllocatedParticles) {
//			_myParticles.kill(myParticle);
//		}
//		_mySubIndex = 0;
//	}
//	
//	public void kill(int theNumberOfParticles) {
//		for(int i = 0; i < theNumberOfParticles && _myAllocatedParticles.size() > 0;i++) {
//			CCGPUParticle myParticle = _myAllocatedParticles.remove(_myAllocatedParticles.size() - 1);
//			if(myParticle != null)_myParticles.kill(myParticle);
//		}
//	}
	
	public void update(final float theDeltaTime) {
		_myParticleWaitingList.update(theDeltaTime);
	}
	
	public int size() {
		return _myNumberOfParticles;
	}
	
	public CCGPUParticle particle(final int theID) {
		return _myActiveParticlesArray[theID - _myStart];
	}
	
	public void fillPositionData(FloatBuffer theBuffer){
		int i = 0;
		for (CCGPUParticle myParticle:allocatedParticles()){
			_myFillArray[i * 3 + 0] = myParticle.position().x;
			_myFillArray[i * 3 + 1] = myParticle.position().y;
			_myFillArray[i * 3 + 2] = myParticle.position().z;
			i++;
		}
		theBuffer.put(_myFillArray, 0, allocatedParticles().size() * 3);
	}
	
	public void fillInfoData(FloatBuffer theBuffer){
		int i = 0;
		for (CCGPUParticle myParticle:allocatedParticles()){
			_myFillArray[i * 3 + 0] = 0;
			_myFillArray[i * 3 + 1] = myParticle.lifeTime();
			_myFillArray[i * 3 + 2] = myParticle.isPermanent() ? 1 : 0;//, myParticle.step();
			i++;
		}
		theBuffer.put(_myFillArray, 0, allocatedParticles().size() * 3);
	}
	
	public void fillVelocityData(FloatBuffer theBuffer){
		int i = 0;
		for (CCGPUParticle myParticle:allocatedParticles()){
			_myFillArray[i * 3 + 0] = myParticle.velocity().x;
			_myFillArray[i * 3 + 1] = myParticle.velocity().y;
			_myFillArray[i * 3 + 2] = myParticle.velocity().z;
			i++;
		}
		theBuffer.put(_myFillArray, 0, allocatedParticles().size() * 3);
	}
	
	public void transferData(CCGraphics g){
		_myParticles.dataTexture().beginDraw();
		_myParticles.initValueShader().start();
		
		_myEmitMesh.draw(g);
		
		_myParticles.initValueShader().end();
		_myParticles.dataTexture().endDraw();
	}
	
	public void setData(CCGraphics theGraphics) {
		
		int myEmitSize = _myAllocatedParticles.size();
		if(myEmitSize == 0)return;

		if(myEmitSize > _myEmitMesh.numberOfVertices()){
			_myVertexBuffer = CCBufferUtil.newDirectFloatBuffer(myEmitSize * 3);
			_myPositionBuffer = CCBufferUtil.newDirectFloatBuffer(myEmitSize * 3);
			_myInfoBuffer = CCBufferUtil.newDirectFloatBuffer(myEmitSize * 3);
			_myVelocityBuffer = CCBufferUtil.newDirectFloatBuffer(myEmitSize * 3);
			_myFillArray = new float[myEmitSize * 3];
		}else{
			_myVertexBuffer.limit(myEmitSize * 3);
			_myPositionBuffer.limit(myEmitSize * 3);
			_myInfoBuffer.limit(myEmitSize * 3);
			_myVelocityBuffer.limit(myEmitSize * 3);
		}
		
		_myVertexBuffer.rewind();
		_myPositionBuffer.rewind();
		_myInfoBuffer.rewind();
		_myVelocityBuffer.rewind();
		
		for (CCGPUParticle myParticle:allocatedParticles()){
			_myVertexBuffer.put(myParticle.x() + 0.5f);
			_myVertexBuffer.put(myParticle.y() + 0.5f);
			_myVertexBuffer.put(0);
		}
		
		fillPositionData(_myPositionBuffer);
		fillInfoData(_myInfoBuffer);
		fillVelocityData(_myVelocityBuffer);
		
		_myVertexBuffer.rewind();
		_myPositionBuffer.rewind();
		_myInfoBuffer.rewind();
		_myVelocityBuffer.rewind();
		
		_myEmitMesh.clearAll();
		_myEmitMesh.vertices(_myVertexBuffer);
		_myEmitMesh.textureCoords(0, _myPositionBuffer, 3);
		_myEmitMesh.textureCoords(1, _myInfoBuffer, 3);
		_myEmitMesh.textureCoords(2, _myVelocityBuffer, 3);
		
		transferData(theGraphics);
		
		allocatedParticles().clear();
	}
}
}; // namespace

#endif
