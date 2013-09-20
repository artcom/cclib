
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
#include <particles/gpuattractor.h>
#include <particles/gpuviscousdrag.h>
#include <particles/gpuyforceblend.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>

using namespace cclib;

class YForceBlendDemo {

    private:

        float _cFieldStrength;
        float _cAttractorStrength;
        float _cAttractorRadius;
        double _myTime;

        GPUParticlesPtr _myParticles;
        GPUIndexParticleEmitterPtr _myEmitter;

        GPUForceFieldPtr _myForceField;
        GPUGravityPtr _myGravity;
        GPUAttractorPtr _myAttractor;
        GPUYForceBlendPtr _myYBlend;

    public:
        bool running;

        YForceBlendDemo() :
            _cFieldStrength(1.46666f),
            _cAttractorStrength(18.53334f),
            _cAttractorRadius(242.0f),
            _myTime(0.0f),
            running(true)
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

            _myForceField = GPUForceField::create(0.0005f, 5, Vector3f(100, 20, 30));
            _myGravity = GPUGravity::create(Vector3f(4,0,0));
            GPUGravityPtr myGravity = GPUGravity::create(Vector3f(-4,0,0));

            _myAttractor = GPUAttractor::create(Vector3f(), 0, 0);
            _myYBlend = GPUYForceBlend::create();
            _myYBlend->initialize(myGravity, _myGravity);

            float y1 = -40.0f;
            float y2 = 40.0f;
            float blendWidth = 100.0f;

            _myYBlend->set("y1", y1);
            _myYBlend->set("y2", y2);
            _myYBlend->set("blendWidth", blendWidth);

            std::vector<GPUForcePtr> myForces;
            myForces.push_back(GPUViscousDrag::create(0.25f));
            // myForces.push_back(_myGravity);
            // myForces.push_back(_myForceField);
            myForces.push_back(_myYBlend);
            myForces.push_back(_myAttractor);

            std::vector<GPUConstraintPtr> myConstraints;
            std::vector<GPUImpulsePtr> myImpulses;

            GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
            _myParticles = GPUParticles::create( myRenderer, myForces, myConstraints, myImpulses, 1000, 1000);
            _myEmitter = GPUIndexParticleEmitter::create(_myParticles);
            _myParticles->addEmitter(_myEmitter);

            Graphics::smooth();
        }

        ~YForceBlendDemo() {
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
            Graphics::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
            Graphics::clear();
            
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            int width, height, mouseX, mouseY = 0;

            glfwGetWindowSize(&width, &height);

            glViewport(0, 0, width, height);
            gluPerspective(60, (float)width/(float)height, 1, 10000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

            glfwGetMousePos( &mouseX, &mouseY );

            // update
            _myTime += 1.0f/30.0f * 0.5f;
            for(int i = 0; i < 1500; i++){
                Vector3f pos = Vector3f(cclib::random<float>(-width/2, width/2),
                                        cclib::random<float>(-height/2, height/2),
                                        cclib::random<float>(-50.0, 50.0));
                Vector3f vel;
                vel.randomize(10.0f);
                Color c(1.0f, 0.0f, 0.0f, 1.0f);
                _myEmitter->emit(c, pos, vel, 10, false);
            }

            _myParticles->update(1.0f/60.0f);

            _myForceField->setStrength(_cFieldStrength);
            _myForceField->setNoiseOffset(Vector3f(0, 0, _myTime));
            _myForceField->setNoiseScale(0.0035f);

            _myAttractor->setStrength(_cAttractorStrength);
            _myAttractor->setRadius(_cAttractorRadius);
            _myAttractor->setPosition( Vector3f(mouseX - width/2.0f, height/2.0f - mouseY, 0.0f) );

            // draw
            Graphics::pointSize(1.0f);

            Graphics::clear();
            Graphics::noDepthTest();
            Graphics::color(1.0f, 1.0f, 1.0f, 50.0f/255.0f);
            Graphics::blend();
            _myParticles->draw();
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
            glfwSwapBuffers();

            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
            glPopMatrix();
        }
};


int main() {
    try {
        YForceBlendDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}



