
#include <GL/glew.h>
#include <GL/glfw.h>
#include <capi/capi.h>
#include <cclib.h>

using namespace cclib;

class CapiDemoStresstest {

    public:
        bool running;
        int counter;
        GLuint texture;

        CapiDemoStresstest() :
            running(true),
            counter(0)
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
             
        }

        ~CapiDemoStresstest() {
            glfwTerminate();
        }

        void update(double theDeltaTime) {
           
            if (counter == 0) {
                std::cout << "Setup Particle System" << std::endl;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                void* texturePtr = (void*)texture;

                cclib_initializeParticleSystem();
                // add some forces
                cclib_addForce("gravity", "gravity");
                cclib_setupParticleSystem(texturePtr);
            }
            
            cclib_updateSimulation();
            counter++;
            
            if (counter == 120) { 
                std::cout << "Teardown Particle System" << std::endl;
                glDeleteTextures(1, &texture);
                
                cclib_teardownParticleSystem();
                counter = 0;
                sleep(3);
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



