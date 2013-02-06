
#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpuparticles.h>
#include <particles/gpuparticlerenderer.h>
#include <particles/gpuparticlepointrenderer.h>
#include <particles/gpuparticleemitter.h>
#include <particles/gpuindexparticleemitter.h>
#include <particles/gpuforce.h>
#include <particles/gpuforcefield.h>
#include <gl/graphics.h>

using namespace cclib;

class EmitDemo {

    private:    
        float _cLifeTime; // = 3f;
        float _cInitVel; // = 3f;
        float _cX; // = 0;
        float _cY; // = 0;
        float _cZ; // = 0;
        float _cGStrength; // = 0;
        GPUForceFieldPtr _myForceField;
        float _cNScale; // = 0;
        float _cNStrength; // = 0;
        GPUParticlesPtr _myParticles;
        GPUIndexParticleEmitterPtr _myEmitter;
    
        int frame;
    
    public:
        bool running;
        
        EmitDemo() :
            _cLifeTime(3.0f), _cInitVel(3.0f), _cX(0.0f), _cY(0.0f), _cZ(0.0f),
            _cGStrength(0.0f), _myForceField(), 
            _cNScale(0.0f), _cNStrength(0.0f),
            _myParticles(), _myEmitter(), 
            frame(0), running(true)
        { 
            if( !glfwInit() )
            {
                running = false;
            }

            glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
            glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
            glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
            // Open an OpenGL window
            if( !glfwOpenWindow( 1200, 800,
                                0,0,0, 0,0,0, GLFW_WINDOW ) )
            {
                glfwTerminate();
                running = false;
            }

            GLenum err = glewInit();
            if (GLEW_OK != err)
            {
                running = false;
            }
            
            setup();
        }

        void setup() {
            std::vector<GPUForcePtr> myForces;
            std::vector<GPUConstraintPtr> myConstraints;
            std::vector<GPUImpulsePtr> myImpulses;

            _myForceField = GPUForceField::create(0.01f, 1.0f, Vector3fPtr(new Vector3f()));
            myForces.push_back(_myForceField);
            
            // GPYConstraint myYConstraint = GPUYConstraint::create(-400, 1.0f, 0f, 0.1f); 
            // myConstraints.add(myYConstraint);
            
            GPUParticlePointRendererPtr myRenderer = GPUParticlePointRenderer::create();
            
            _myParticles = GPUParticles::create( myRenderer, myForces, myConstraints, myImpulses, 800, 800);
            _myEmitter = GPUIndexParticleEmitter::create(_myParticles);
            _myParticles->addEmitter(_myEmitter);
        }

        ~EmitDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            int width, height, mouseX, mouseY = 0;
            glfwGetMousePos( &mouseX, &mouseY );
            glfwGetWindowSize( &width, &height );
                
            float x = mouseX - width/2;
            float y = height/2 - mouseY;
            for(int i = 0; i < 1000; i++){
                Vector3fPtr vel = Vector3fPtr(new Vector3f());
                Vector3fPtr pos = Vector3fPtr(new Vector3f(x, y, 0.0f));
                
                vel->randomize();
                vel->scale(_cInitVel);
                _myEmitter->emit(pos, vel, _cLifeTime);
            }
            
            // _myForceField.noiseScale(_cNScale);
            // _myForceField.strength(_cNStrength);
            _myParticles->update(theDeltaTime);
		
            //  draw
            Graphics::clear();
            // Graphics::noDepthTest();
            // Graphics::color(255, 50);
            // Graphics::blend(GL_ADD);
            
            _myParticles->draw();
            Graphics::noBlend();

            glfwSwapBuffers();

            
            std::cout << "frame: " << frame++ << std::endl;
            // running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
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



