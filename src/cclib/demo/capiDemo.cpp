
#include <GL/glew.h>
#include <GL/glfw.h>
#include <capi/capi.h>
#include <cclib.h>
#include <unistd.h>

using namespace cclib;

class CapiDemoStresstest {

    public:
        bool running;
        int counter;
        GLuint texture;
        int wrapperId;
    
    
        void teardown() {
            cclib_teardownParticleSystem(wrapperId);
        }
 
        CapiDemoStresstest() :
            running(true),
            counter(0),
            wrapperId(0)
        {
            if( !glfwInit() ) {
                running = false;
            }

            // Open an OpenGL window
            if( !glfwOpenWindow( 1400, 750, 0,0,0, 0,0,0, GLFW_WINDOW ) ) {
                glfwTerminate();
                running = false;
            }

            glfwSwapInterval(1);

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            GLenum err = glewInit();
            if (GLEW_OK != err) {
                running = false;
            }
            
            setup();
        }

        void setup() {
             
        }

        ~CapiDemoStresstest() {
            glfwTerminate();
        }

        void setupParticleSystem() {
            void* texturePtr = (void*)texture;

            wrapperId = cclib_initializeParticleSystem();
            cclib_addForce(wrapperId, "targetforce", "targetforce");
            cclib_addForce(wrapperId, "forcefield", "noise");
            cclib_addForce(wrapperId, "viscousdrag", "drag");
            
            cclib_addEmitter(wrapperId, "targetemitter", "lineEmitter1");
            cclib_addEmitter(wrapperId, "targetemitter", "lineEmitter2");
            cclib_addEmitter(wrapperId, "targetemitter", "lineEmitter3");

            cclib_addEmitter(wrapperId, "targetemitter", "targetEmitter1");
            cclib_addEmitter(wrapperId, "targetemitter", "targetEmitter2");
            cclib_addEmitter(wrapperId, "targetemitter", "targetEmitter3");
    
            cclib_setupParticleSystem(wrapperId,  texturePtr);
            cclib_setInfoTexture(wrapperId,  texturePtr);
            cclib_setColorTexture(wrapperId,  texturePtr);
        }
    
        void update(double theDeltaTime) {
           
            if (counter == 0) {
                setupParticleSystem();
            }
 
            
            cclib_updateParameterComponentReference(wrapperId, "lineEmitter1", "targetForce", "targetforce");
            cclib_updateParameterComponentReference(wrapperId, "lineEmitter2", "targetForce", "targetforce");
            cclib_updateParameterComponentReference(wrapperId, "lineEmitter3", "targetForce", "targetforce");

            cclib_updateParameterComponentReference(wrapperId, "targetEmitter1", "targetForce", "targetforce");
            cclib_updateParameterComponentReference(wrapperId, "targetEmitter2", "targetForce", "targetforce");
            cclib_updateParameterComponentReference(wrapperId, "targetEmitter3", "targetForce", "targetforce");
        
            
            cclib_updateSimulation(wrapperId);
            counter++;
 
            if (counter > 200) {
                counter = 0;
                teardown();
                std::cout << "teardown.. " << std::endl;
            }
        }
};


int main() {
    try {
        CapiDemoStresstest demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}



