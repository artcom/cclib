
#include <gl/gpuparticles.h>

using namespace cclib;

GPUParticles::GPUParticles( GPUParticleRenderer::Ptr theRender,
        std::vector<GPUForce::Ptr> & theForces, std::vector<GPUConstraint::Ptr> & theConstraints, 
        std::vector<GPUImpulse::Ptr> % theImpulse, int theWidth, int theHeight) :
    _myForces(theForces), _myConstraints(theConstraints),
    _myImpulses(theImpulse),_myWidth(theWidth), _myHeight(theHeight), _myCurrentTime(0.0)
{
    for (std::vector<GPUForces::Ptr>::size_type f=0; f<theForces.size(); f++) {
        theForces[f]->setSize(theWidth, theHeight);
    }

    _myInitValue01Shader = new CGShader(null,CCIOUtil.classPath(this, "shader/initvalue01.fp"));
    _myInitValue01Shader.load();

    _myInitValue0Shader = new CGShader(null,CCIOUtil.classPath(this, "shader/initvalue.fp"));
    _myInitValue0Shader.load();

    _myCurrentDataTexture = new CCShaderTexture(32, 4, 3, _myWidth, _myHeight);
    Graphics::clearColor(0.0f, 0.0f, 1.0ff);
    _myCurrentDataTexture.beginDraw(0);
    Graphics::clear();
    _myCurrentDataTexture.endDraw();
    _myCurrentDataTexture.beginDraw(1);
    Graphics::clear();
    _myCurrentDataTexture.endDraw();
    Graphics::clearColor(0);

    _myDestinationDataTexture = new CCShaderTexture(32, 4, 3, _myWidth, _myHeight);

    Graphics::noBlend();
    _myCurrentDataTexture.beginDraw();
    _myInitValue01Shader.start();

    Graphics::beginShape(GL_POINTS);
    for (int i = 0; i < _myWidth * _myHeight; i++){
        Graphics::textureCoords(0, 0f, 0f, 0f);
        Graphics::textureCoords(1, 0f, 0f, 0f);
        Graphics::vertex(i % _myWidth,i / _myWidth);
    }
    Graphics::endShape();

    _myInitValue01Shader.end();
    _myCurrentDataTexture.endDraw();

    _myParticleRender = theRender;
    _myParticleRender.setup(Ptr(this));
    _myUpdateShader = new CCGPUUpdateShader(Ptr(this),theForces, theConstraints, theImpulse, _myWidth, _myHeight);

    reset();
}
    
GPUParticles::Ptr 
GPUParticles::create( GPUParticleRenderer::Ptr theRender,
        std::vector<GPUForce::Ptr> & theForces, std::vector<GPUConstraint::Ptr> & theConstraints, 
        std::vector<GPUImpulse::Ptr> & theImpulse, int theWidth, int theHeight) 
{
    Ptr ptr = Ptr(new GPUParticles(theRender, theForces, theConstraints, theImpulse, theWidth, theHeight)); 
    return Ptr;
}

void 
GPUParticles::addEmitter(GPUParticleEmitter::Ptr theEmitter) {
    _myEmitter.push_back(theEmitter);
}

Shader::Ptr 
GPUParticles::initValueShader() {
    return _myInitValue01Shader;
}

double 
GPUParticles::currentTime() {
    return _myCurrentTime;
}

void 
GPUParticles::reset(){
    _myCurrentDataTexture->clear();

    _myCurrentDataTexture->beginDraw();
    _myInitValue01Shader->start();

    Graphics::beginShape(GL_POINTS);
    for (int i = 0; i < _myWidth * _myHeight; i++){
        Graphics::textureCoords(0, FLT_MAX, FLT_MAX, FLT_MAX);
        Graphics::textureCoords(1, 1, 1, 1);
        Graphics::vertex(i % _myWidth,i / _myWidth);
    }
    Graphics::endShape();

    _myInitValue01Shader->end();
    _myCurrentDataTexture->endDraw();

    for (unsigned int i=0; i<_myForces.size(); i++)
        _myForces[i]->reset();
}
}

int 
GPUParticles::width() {
    return _myWidth;
}

int 
GPUParticles::height() {
    return _myHeight;
}

int 
GPUParticles::size() {
    return _myWidth * _myHeight;
}

ShaderTexture::Ptr 
GPUParticles::dataTexture() {
    return _myCurrentDataTexture;
}

Vector3f::Ptr 
GPUParticles::position(GPUParticle::Ptr theParticle) {
    FloatBuffer myResult = _myCurrentDataTexture.getData(theParticle->x(), theParticle->y(), 1, 1);
    return Vector3f::Ptr(new Vector3f(myResult.get(), myResult.get(), myResult.get()));
}

ShaderTexture::Ptr 
GPUParticles::destinationDataTexture() {
    return _myDestinationDataTexture;
}

void 
GPUParticles::setPosition(int theIndex, Vector3f::Ptr thePosition) {
    _myPositionUpdates[theIndex] = thePosition;
}

void 
GPUParticles::updateLifecyle(GPUParticle::Ptr theParticle) {
    _myLifetimeUpdates.push_back(theParticle);
}

void 
GPUParticles::updateManualPositionChanges() {

    if (_myPositionUpdates.size() == 0) {
        return;
    }

    // Render manually changed positions into the texture.
    _myCurrentDataTexture->beginDraw(0);
    _myInitValue0Shader->start();

    Graphics::beginShape(GL_POINTS);

    Iterator<Entry<Integer, CCVector3f>> it = _myPositionUpdates.entrySet().iterator();

    while (it.hasNext()) {
        Map.Entry<Integer, CCVector3f> pairs = (Map.Entry<Integer, CCVector3f>)it.next();

        Graphics::textureCoords(0, pairs.getValue());
        Graphics::vertex(pairs.getKey() % _myWidth, 
                pairs.getKey() / _myWidth);
    }

    Graphics::endShape();

    _myInitValue0Shader->end();
    _myCurrentDataTexture->endDraw();

    _myPositionUpdates->clear();
}

void 
GPUParticles::initializeNewParticles() {
    // Render current position into texture.

    for (unsigned int i=0; i<_myEmitter.size(); i++) {
        _myEmitter[i]->setData();
    }
}

void 
GPUParticles::changeStates() {
    // XXX originally commented out
    //		_myCurrentDataTexture.beginDraw(1);
    //		_myInitValue0Shader.start();
    //		Graphics::beginShape(CCDrawMode.POINTS);
    //		for(CCGPUParticleEmitter myEmitter:_myEmitter) {
    //			for (CCGPUParticle myChangedParticle:myEmitter.stateChangedParticles()){
    //				Graphics::textureCoords(0, myChangedParticle.age(), myChangedParticle.lifeTime(), myChangedParticle.isPermanent() ? 1 : 0, myChangedParticle.step());
    //				Graphics::vertex(myChangedParticle.x(),myChangedParticle.y());
    //			}
    //			myEmitter.stateChangedParticles().clear();
    //		}
    //		Graphics::endShape();
    //		
    //		_myInitValue0Shader.end();
    //		_myCurrentDataTexture.endDraw();
}

void 
GPUParticles::beforeUpdate() {
    initializeNewParticles();
    changeStates();
}

void 
GPUParticles::cleanUpParticles() {
    // XXX originally commented out
    //		if(_myActiveParticles.size() <= 0)
    //			return;
    //		
    //		_myCurrentPositionTexture.beginDraw(1);
    //		_myInitValue1Shader.start();
    //		Graphics::beginShape(CCDrawMode.POINTS);
    //				
    //		while (_myActiveParticles.peek() != null && _myActiveParticles.peek().timeOfDeath() < _myCurrentTime){
    //			CCGPUParticle myParticle = _myActiveParticles.poll();
    //			if(myParticle.index == -1) continue;
    //			_myAvailableIndices.add(myParticle.index);
    //			_myActiveParticlesArray[myParticle.index].index = -1;
    //			
    //			Graphics::textureCoords(0, Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
    //			Graphics::textureCoords(1, 0, 0, 1, 0);
    //			Graphics::vertex(myParticle.x() + 0.5f, myParticle.y() + 0.5f);
    //		}
    //		
    //		Graphics::endShape();
    //		_myInitValue1Shader.end();
    //		_myCurrentPositionTexture.endDraw();
}

void 
GPUParticles::afterUpdate(){
    updateManualPositionChanges();
    //		updateManualLifetimeReset();
    cleanUpParticles();
}

void 
GPUParticles::update(float theDeltaTime) {
    if(theDeltaTime <= 0) {
        return;
    }

    for(CCGPUParticleEmitter myEmitter:_myEmitter) {
        myEmitter.update(theDeltaTime);
    }

    Graphics::noBlend();
    beforeUpdate();

    for (unsigned int f=0; f<_myForces.size(); f++) {
        _myForces[f]->update(theDeltaTime);
    }

    for (unsigned int c=0; c<_myConstraints.size(); c++) {
        myConstraints[c]->update(theDeltaTime);
    }

    for (unsigned int i=0; i<_myImpulses.size(); i++) {
        _myImpulses[i]->update(theDeltaTime);
    }

    _myUpdateShader->positions(_myCurrentDataTexture);
    _myUpdateShader->deltaTime(theDeltaTime);
    _myUpdateShader->start();
    _myDestinationDataTexture->draw();
    _myUpdateShader->end();

    swapDataTextures();

    afterUpdate();
    _myCurrentTime += theDeltaTime;
    _myParticleRender->update(theDeltaTime);
    Graphics::blend();
}

void 
GPUParticles::swapDataTextures() {
    ShaderTexture::Ptr myTemp = _myDestinationDataTexture;
    _myDestinationDataTexture = _myCurrentDataTexture;
    _myCurrentDataTexture = myTemp;
}

void 
GPUParticles::draw() {
    _myParticleRender->draw();
}

GPUParticleRenderer::Ptr 
GPUParticles::renderer() {
    return _myParticleRender;
}


