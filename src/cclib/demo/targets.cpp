#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpunoise.h>
#include <stringified_shaders/initvalue.fp.h>
#include <gl/cgshader.h>
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
        CGShaderPtr _myInitValueShader;
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

            _myForceField = GPUForceField::create(0.01, 1, Vector3f());
            _myTargetForce = GPUTargetForce::create();
            _myViscousDrag = GPUViscousDrag::create();
            _myViscousDrag->setDrag(0.28);
            
			_myTimeBlendForce = GPUTimeForceBlend::create();
            float startTime = 3.0f;
            float endTime = 5.0f;
            float power = 1;
			
            _myTimeBlendForce->set<float>("startTime", startTime);
			_myTimeBlendForce->set<float>("endTime", endTime);
            _myTimeBlendForce->setBlend(0, 1.0f, 0.0f);
            _myTimeBlendForce->setBlend(1, 0.0f, 1.0f);
			_myTimeBlendForce->set<float>("power", power);
			
            //   _myTimeBlendForce->initialize(_myTargetForce, _myForceField);
            _myTimeBlendForce->initialize(_myForceField, _myTargetForce);

            std::vector<GPUForcePtr> myForces;
            std::vector<GPUImpulsePtr> myImpulses;
            std::vector<GPUConstraintPtr> myConstraints;

			myForces.push_back(_myTimeBlendForce);

//			myForces.push_back(_myTargetForce);
            myForces.push_back(_myViscousDrag);
//			myForces.push_back(_myForceField);
            
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

            _myInitValueShader = CGShader::create(vfiles, ffiles);
			_myInitValueShader->load();
            
            ///
			Vector3f myVelocity;
			
			_myInitValueShader->start();
			_myTargetBuffer->beginDraw();
            
            Graphics::beginShape(GL_POINTS);
            
			for (unsigned int i=0; i<700; i++) {
                for (unsigned int j=0; j<700; j++) {
                    Vector3f myVelocity;
                    myVelocity.randomize();
                    myVelocity.scale(3);
                                  
                    GPUParticlePtr myParticle = _myEmitter->emit(Vector3f(0, 0, 0), myVelocity, 50, false);
                
                    Graphics::textureCoords(0, 350 - (float)i, 350 - (float)j);
                    Graphics::vertex(myParticle->x() + 0.5f, myParticle->y() + 0.5f);
                }
            }
            
            Graphics::endShape();
			_myTargetBuffer->endDraw();
			_myInitValueShader->end();
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
            gluPerspective(60, (float)width/(float)height, 0.1, 1000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            
            ///////

            time += theDeltaTime;

            // draw
            Graphics::clear();
            
            // _myArcball.draw(g);
            Graphics::color(1.0f, 1.0f, 1.0f, 0.5f);
            Graphics::noDepthTest();
            
            Graphics::blend(BLEND_MODE_ADD);
            
            
////////////
            _myParticles->update(theDeltaTime);
			myREs += theDeltaTime;
					
			_myTargetForce->set<float>("strength", _cTargetStrength);
			_myTargetForce->set<float>("lookAhead", _cLookAhead);
			_myTargetForce->set<float>("maxForce", _cMaxForce);
			_myTargetForce->set<float>("nearDistance", _cTargetNearDistance);
			_myTargetForce->set<float>("nearMaxForce", _cTargetNearMaxForce);
					
			_myOffset += theDeltaTime * _cSpeed;
			_myForceField->set<float>("noiseScale", _cNScale);
			_myForceField->set<float>("strength", _cStrength);
            Vector3f offset = Vector3f(0, 0, _myOffset);
			_myForceField->set<Vector3f>("noiseOffset", offset);
			
            _myTimeBlendForce->set<float>("power", _cLifeTimeBlendPow);
			
		//	_myViscousDrag->setDrag(_cDrag);
            
 /////////////           
            Graphics::blend();
            Graphics::noDepthTest();
        
            Graphics::pointSize(2.0f);
			_myParticles->draw();
            
            Graphics::depthTest();

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


