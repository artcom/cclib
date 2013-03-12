#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpunoise.h>
#include <stringified_shaders/initvalue.fp.h>
#include <gl/shader.h>
#include <gl/mesh.h>
#include <gl/vbomesh.h>
#include <gl/graphics.h>
#include <gl/shaderbuffer.h>

#include <particles/gpugravity.h>
#include <particles/gpuviscousdrag.h>
#include <particles/gpuforcefield.h>
#include <particles/gpuparticlepointrenderer.h>
#include <particles/gpuparticle.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpucombinedforce.h>
#include <particles/gputargetforce.h>
#include <particles/gputimeforceblend.h>

using namespace cclib;

class TargetsDemo {

    private:    
		GPUIndexParticleEmitterPtr _myEmitter;
		GPUParticlesPtr _myParticles;
		
		GPUForceFieldPtr _myForceField;
		GPUGravityPtr _myGravity;
		GPUViscousDragPtr _myViscousDrag;
		GPUCombinedForcePtr _myCombinedForce;
		GPUTargetForcePtr _myTargetForce;
		ShaderBufferPtr _myTargetBuffer;
		GPUTimeForceBlendPtr _myTimeBlendForce;

        float _myRadius;
        // TextureDataPtr _myTargetTextureData;
        ShaderPtr _myInitValueShader;
        float myREs;
        int frame;
        double time;
        float _myOffset;
    
    // parameters
        float _cOpenSpeed;
        float _cStartVelocity;
        float _cStartRadius;
        float _cRadiusRange;
        float _cParticlePerimeterDensity;
    
        float _cDrag;
    
        float _cTargetStrength;
        float _cLookAhead;
        float _cMaxForce;
        float _cTargetNearDistance;
        float _cTargetNearMaxForce;
        float _cLookUpLookAhead;
    
        float _cLifeTimeBlendPow;
    
        float _cNScale;
        float _cStrength;
        float _cSpeed;
    
        float _cEmitX;
        float _cEmitY;
        float _cEmitRandomPos;
        float _cEmitRandomVel;
    
        float _cTargetX;
        float _cTargetY;
        float _cEmitTargetLookAhead;
    
    public:
        bool running;
        
        TargetsDemo() :
            frame(0), running(true),
            myREs(1), time(0),
            _cOpenSpeed(86.6), _cStartVelocity(0.93), _cStartRadius(0), _cRadiusRange(1000),
            _cParticlePerimeterDensity(1.0280), _cDrag(0.2866), _cTargetStrength(1.0), _cLookAhead(10.0),
            _cMaxForce(1.0), _cTargetNearDistance(16.0), _cTargetNearMaxForce(0.3333334), _myOffset(1.73333),
            _cLookUpLookAhead(1.7333), _cLifeTimeBlendPow(0.1999), _cNScale(0.04), _cStrength(0.46666660), _cSpeed(0.56), _cEmitX(13.33),
            _cEmitY(-80.0), _cEmitRandomPos(14.0), _cEmitRandomVel(0), _cTargetX(-240.0), _cTargetY(26.666), _cEmitTargetLookAhead(0.9)
{
            if(!glfwInit()) {
                running = false;
            }

            // Open an OpenGL window
            if(!glfwOpenWindow( 1400, 750, 0, 0, 0, 0, 0, 0, GLFW_WINDOW )) {
                glfwTerminate();
                running = false;
            }

            GLenum err = glewInit();
            if(GLEW_OK != err){
                running = false;
            }
            
            setup();
        }

        void setup() {

            _myGravity = GPUGravity::create(Vector3f());
            _myForceField = GPUForceField::create(0.01, 1, Vector3f());
            _myViscousDrag = GPUViscousDrag::create(0.1f);	
            _myTargetForce = GPUTargetForce::create();

            std::vector<GPUForcePtr> myCombinedForces;
            myCombinedForces.push_back(_myGravity);
			myCombinedForces.push_back(_myForceField);
			
            _myCombinedForce = GPUCombinedForce::create(myCombinedForces);
			
			_myTimeBlendForce = GPUTimeForceBlend::create(0, 4, _myCombinedForce, _myTargetForce);
			_myTimeBlendForce->setBlend(0.005f, 1.0f);
			_myTimeBlendForce->setPower(6);
				
			// _myTargetTextureData = CCTextureIO.newTextureData("squarepusher.png");

            std::vector<GPUForcePtr> myForces;
            std::vector<GPUImpulsePtr> myImpulses;
            std::vector<GPUConstraintPtr> myConstraints;

//			myForces.push_back(_myTimeBlendForce);
			myForces.push_back(_myTargetForce);
			myForces.push_back(_myForceField);
			myForces.push_back(_myViscousDrag);
			
            GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
            _myParticles = GPUParticles::create( myRenderer, myForces, myConstraints, myImpulses, 700, 700);

            _myEmitter = GPUIndexParticleEmitter::create(_myParticles);
            _myParticles->addEmitter(_myEmitter);
            
            _myTargetBuffer = ShaderBuffer::create(_myParticles->width(), _myParticles->height(), 16, 4);
			
            _myTargetBuffer->beginDraw();
            Graphics::clear();
            _myTargetBuffer->endDraw();
			
            _myTargetForce->addTargetSetup( _myTargetBuffer );
			
            std::vector<std::string> vfiles, ffiles; 
            ffiles.push_back(initvalue_fp);

            _myInitValueShader = Shader::create(vfiles, ffiles);
			_myInitValueShader->load();
        }

        ~TargetsDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            int width, height = 0;
            glfwGetWindowSize(&width, &height);
            glViewport(0, 0, width, height);
            gluPerspective(60, (float)width/(float)height, 1, 10000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            
            ///////

            time += theDeltaTime;
            // if(time > max ){
            //     time -= max;
            //     _myOldOffset = _myNewOffset;
            //     _myNewOffset = CCVecMath.random(-10, 10, -10, 10, -10, 10);
            // }

            // blend = (CCMath.cos((time / max) * CCMath.PI + CCMath.PI) +1) / 2;
            // _myOffset = CCVecMath.blend(blend, _myOldOffset, _myNewOffset);
           
            // draw

            Graphics::clear();
            // _myArcball.draw(g);
            Graphics::color(1.0f, 1.0f, 1.0f, 0.05f);
            Graphics::noDepthTest();
            
            Graphics::blend(BLEND_MODE_ADD);
            
            
////////////
            _myParticles->update(theDeltaTime);
			myREs += theDeltaTime;
			_myRadius += theDeltaTime * _cOpenSpeed;
			if(_myRadius > _cStartRadius + _cRadiusRange){
				_myRadius = _cStartRadius;
			}
			
			Vector3f myVelocity;
			
			float myPerimeter = 2.0 * M_PI * _myRadius;
			float myNumberOfParticles = myPerimeter / _cParticlePerimeterDensity;
			float myStep = 360 / myNumberOfParticles;
			
			_myInitValueShader->start();
			_myTargetBuffer->beginDraw();
			
			// float myTargetX2 = _myTargetTextureData->width() + _cTargetX;
			// float myTargetY2 = _myTargetTextureData->height() + _cTargetY;
			float myTargetX2 = 400 + _cTargetX;
			float myTargetY2 = 400 + _cTargetY;
			
            Graphics::beginShape(GL_POINTS);
			for(float angle = 0; angle < 360; angle += myStep){
				float radAng = radians(angle);
				float myX = sin(radAng) * _myRadius;
				float myY = cos(radAng) * _myRadius;
				
				myVelocity.set(myX, myY, 0);
				myVelocity.normalize();
				myVelocity.scale(_cOpenSpeed * _cStartVelocity);
				myVelocity = myVelocity + Vector3f(
                    random(-_cEmitRandomVel, _cEmitRandomVel), random(-_cEmitRandomVel, _cEmitRandomVel), 0);
				
				myX += _cEmitX + random(-_cEmitRandomPos, _cEmitRandomPos);
				myY += _cEmitY + random(-_cEmitRandomPos, _cEmitRandomPos);
				
				float myTargetX = myX + myVelocity.x() * _cEmitTargetLookAhead;
				float myTargetY = myY + myVelocity.y() * _cEmitTargetLookAhead;
				if(myTargetX <= _cTargetX || myTargetX >= myTargetX2 || myTargetY <= _cTargetY || myTargetY >= myTargetY2){
					continue;
				}
				
				int myColorLookUpX = (int)(myTargetX - _cTargetX);
				int myColorLookUpY = (int)(myTargetY - _cTargetY);
				
				Color myParticleColor(1.0f , 1.0f, 1.0f, 1.0f); // = _myTargetTextureData->getPixel(myColorLookUpX, myColorLookUpY);
				// GPUParticlePtr myParticle = _myEmitter->emit(myParticleColor, Vector3f(myX, myY,0), myVelocity, 10, false);
				GPUParticlePtr myParticle = _myEmitter->emit(Vector3f(myX, myY,0), myVelocity, 10, false);
                
                Graphics::textureCoords(0, myTargetX, myTargetY);
				Graphics::vertex(myParticle->x() + 0.5f, myParticle->y() + 0.5f);
			}
            
            Graphics::endShape();
			_myTargetBuffer->endDraw();
			_myInitValueShader->end();
			
			_myTargetForce->setStrength(_cTargetStrength);
			_myTargetForce->setLookAhead(_cLookAhead);
			_myTargetForce->setMaxForce(_cMaxForce);
			_myTargetForce->setNearDistance(_cTargetNearDistance);
			_myTargetForce->setNearMaxForce(_cTargetNearMaxForce);
					
			_myOffset += theDeltaTime * _cSpeed;
			_myForceField->setNoiseScale(_cNScale);
			_myForceField->setStrength(_cStrength);
			_myForceField->setNoiseOffset(Vector3f(0, 0, _myOffset));
			_myTimeBlendForce->setPower(_cLifeTimeBlendPow);
			
			_myViscousDrag->setDrag(_cDrag);
            
 /////////////           
            Graphics::noBlend();

        	// g.ellipse(_cEmitX, _cEmitY, 20,20);
			// g.ellipse(_cTargetX, _cTargetY, 20,20);
            Graphics::pointSize(0.1f);
			_myParticles->draw();
            
            //
            glfwSwapBuffers();
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
            glPopMatrix();
        }
};


int main() {
    try {
        TargetsDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}


