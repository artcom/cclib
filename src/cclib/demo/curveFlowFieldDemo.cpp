
#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpuparticles.h>
#include <particles/gpuparticlerenderer.h>
#include <particles/gpuparticlepointrenderer.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuforce.h>
#include <particles/gpuforcefield.h>
#include <particles/gpugravity.h>
#include <particles/gpucurvefield.h>
#include <particles/gpuattractor.h>
#include <particles/gpuviscousdrag.h>
#include <gl/shadertexture.h>
#include <gl/graphics.h>

using namespace cclib;

class CurveFlowFieldDemo {

    private:    

        float _cFieldStrength;
        float _cAttractorStrength;
        float _cAttractorRadius;
        float _cGravityStrength;
        float _cCurveStrength;
        float _cCurveSpeed;
        float _cPrediction;
        float _cCurveNoiseScale;
        float _cCurveOuputScale;
        float _cCurveRadius;
        float _cEmitRadius;
        double _myTime;

        GPUParticlesPtr _myParticles;
        GPUIndexParticleEmitterPtr _myEmitter;
        
        GPUCurveFieldPtr _myCurveField; 
        GPUForceFieldPtr _myForceField; 
        GPUGravityPtr _myGravity; 
        GPUAttractorPtr _myAttractor;
    
        int frame;
    
    public:
        bool running;
        
        CurveFlowFieldDemo() :
            _cFieldStrength(0.46666f),
            _cAttractorStrength(8.53334f),
            _cAttractorRadius(142.0f),
            _cGravityStrength(0.6666f),
            _cCurveStrength(10.0f),
            _cCurveSpeed(0.046667f),
            _cPrediction(0.253333f),
            _cCurveNoiseScale(0.53333f),
            _cCurveOuputScale(30.6666f),
            _cCurveRadius(96.0f),
            _cEmitRadius(100.3333f),
            _myTime(0.0f),
            frame(0), running(true)
        { 
            if( !glfwInit() ) {
                running = false;
            }

            // Open an OpenGL window
            if( !glfwOpenWindow( 1400, 750, 0,0,0, 0,0,0, GLFW_WINDOW ) ) {
                glfwTerminate();
                running = false;
            }

            GLenum err = glewInit();
            if (GLEW_OK != err) {
                running = false;
            }
            
            setup();
        }

        void setup() {
            
            _myCurveField = GPUCurveField::create();
            _myForceField = GPUForceField::create(0.005f, 1, Vector3fPtr(new Vector3f(100, 20, 30)));
            _myGravity = GPUGravity::create(Vector3fPtr(new Vector3f(150,0,0)));
            _myAttractor = GPUAttractor::create(Vector3fPtr(new Vector3f()), 0, 0);
                
            std::vector<GPUForcePtr> myForces;
            myForces.push_back(GPUViscousDrag::create(0.25f));
            // myForces.push_back(_myCurveField);
            myForces.push_back(_myGravity);
            myForces.push_back(_myForceField);
            myForces.push_back(_myAttractor);
           
            std::vector<GPUConstraintPtr> myConstraints;
            std::vector<GPUImpulsePtr> myImpulses;

            GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
            _myParticles = GPUParticles::create( myRenderer, myForces, myConstraints, myImpulses, 1000, 1000);
            _myEmitter = GPUIndexParticleEmitter::create(_myParticles);
            _myParticles->addEmitter(_myEmitter);
            
            Graphics::smooth();
        }

        ~CurveFlowFieldDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            int width, height, mouseX, mouseY = 0;
            
            glfwGetWindowSize(&width, &height);
            
            glViewport(0, 0, width, height);
            gluPerspective(60, (float)width/(float)height, 1, 10000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            
            glfwGetMousePos( &mouseX, &mouseY );
            
            float x = mouseX - width/2;
            float y = height/2 - mouseY;
       
            // update
            
            _myTime += 1.0f/30.0f * 0.5f;
            for(int i = 0; i < 1500; i++){
                Vector3fPtr pos = Vector3fPtr( new Vector3f(cclib::random<float>(-width/2, width/2),
                            cclib::random<float>(-_cEmitRadius, _cEmitRadius), 
                            cclib::random<float>(-50.0, 50.0))); 
                Vector3fPtr vel = Vector3fPtr( new Vector3f() );
                vel->randomize(10.0f);
                
                _myEmitter->emit( pos, vel, 10, false );
            }

            _myParticles->update(1.0f/60.0f);

            _myGravity->strength(_cGravityStrength);
            
            _myForceField->strength(_cFieldStrength);
            _myForceField->noiseOffset(Vector3fPtr(new Vector3f(0, 0, _myTime)));
            _myForceField->noiseScale(0.0025f);

            _myAttractor->strength(_cAttractorStrength);
            _myAttractor->radius(_cAttractorRadius);
            _myAttractor->position()->x( mouseX - width/2 );
            _myAttractor->position()->y( height/2 - mouseY );

            _myCurveField->strength(_cCurveStrength);
            _myCurveField->outputScale(_cCurveOuputScale);
            _myCurveField->speed(_cCurveSpeed);
            _myCurveField->scale(_cCurveNoiseScale / 100);
            _myCurveField->radius(_cCurveRadius);

            _myCurveField->prediction(_cPrediction);
                
            // draw

            Graphics::pointSize(1.0f);

            Graphics::clear();
            Graphics::noDepthTest();
            // Graphics::pushMatrix();
            // _myArcball.draw(g);
            Graphics::color(1.0f, 1.0f, 1.0f, 15.0f/255.0f);
            Graphics::blend();
            //		g.pointSprite(_mySpriteTexture);
            //		g.smooth();
            Graphics::blend();
            _myParticles->draw();
            //		g.noSmooth();
            //		g.noPointSprite();
            // g.popMatrix();
            
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
            // 
            
            glfwSwapBuffers();

            if (++frame % 60 == 0) {
                std::cout << "frame: " << frame << std::endl;
            }
            
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
            glPopMatrix();
        }
};


int main() {
    try {
        CurveFlowFieldDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}



