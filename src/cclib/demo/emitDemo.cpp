
#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpuparticles.h>
#include <particles/gpuparticlerenderer.h>
#include <particles/gpuparticlepointrenderer.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuforce.h>
#include <particles/gpuforcefield.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>

using namespace cclib;

class EmitDemo {

    private:    
        float _cLifeTime; 
        float _cInitVel;
        float _cX;
        float _cY;
        float _cZ;
        float _cGStrength;
        GPUForceFieldPtr _myForceField;
        float _cNScale;
        float _cNStrength;
        GPUParticlesPtr _myParticles;
        GPUIndexParticleEmitterPtr _myEmitter;
    
        int frame;
    
    public:
        bool running;
        
        EmitDemo() :
            _cLifeTime(8.0f), _cInitVel(15.0f), _cX(0.0f), _cY(0.0f), _cZ(0.0f),
            _cGStrength(0.0f), _myForceField(), 
            _cNScale(0.026f), _cNStrength(0.31f),
            _myParticles(), _myEmitter(), 
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
            std::vector<GPUForcePtr> myForces;
            std::vector<GPUConstraintPtr> myConstraints;
            std::vector<GPUImpulsePtr> myImpulses;

            _myForceField = GPUForceField::create(0.01f, 1.0f, Vector3f());
            myForces.push_back(_myForceField);
            
            // GPYConstraint myYConstraint = GPUYConstraint::create(-400, 1.0f, 0f, 0.1f); 
            // myConstraints.add(myYConstraint);
            
            GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
            
            _myParticles = GPUParticles::create( myRenderer, myForces, myConstraints, myImpulses, 1000, 1000);
            _myEmitter = GPUIndexParticleEmitter::create(_myParticles);
            _myParticles->addEmitter(_myEmitter);
            Graphics::smooth();
        }

        ~EmitDemo() { 
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
            
        
            Graphics::clear();
            Graphics::pointSize(2.0f);
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
            
            for(int i = 0; i < 1000; i++) {
                Vector3f vel = Vector3f();
                Vector3f pos = Vector3f(x, y, 0.0f);
            
                vel.randomize(_cInitVel);
                _myEmitter->emit(pos, vel, _cLifeTime);
            }
        
            _myForceField->setNoiseScale(_cNScale);
            _myForceField->setStrength(_cNStrength);
            _myParticles->update(theDeltaTime);
		            
            //  draw
            Graphics::noDepthTest();
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f/128.0f);
            Graphics::blend(BLEND_MODE_ADD);
            
            _myParticles->draw();
                     
            Graphics::noBlend();
      
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
        EmitDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}



