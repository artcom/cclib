#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpunoise.h>
// #include <stringified_shaders/simplex.fp.h>
#include <gl/cgshader.h>
#include <gl/mesh.h>
#include <gl/vbomesh.h>
#include <gl/graphics.h>
#include "fragmentNoise.h"

using namespace cclib;

class FragmentNoiseDemo {

    private:    
        CGShaderPtr _myShader;
        CGparameter _myNoiseScaleParameter;
        CGparameter _myNoiseOffsetParameter;

    public:
        float time;
    bool running;

        FragmentNoiseDemo() :
            time(0),
            running(true)
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
            Graphics::clearColor(0);
            
            std::vector<std::string> ffiles;
            ffiles.push_back(simplex_fp);
            ffiles.push_back(simplexnoisefragment_fp);
            
            std::vector<std::string> noFiles;
            
            _myShader = CGShader::create(noFiles, ffiles);
            _myShader->load();

            _myNoiseScaleParameter = _myShader->fragmentParameter("noiseScale");
            _myNoiseOffsetParameter = _myShader->fragmentParameter("noiseOffset");
       
            GPUNoise::attachFragmentNoise(_myShader.get());
        }

        ~FragmentNoiseDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            time += theDeltaTime;
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            int width, height = 0;
            glfwGetWindowSize(&width, &height);
            glViewport(0, 0, width, height);
            gluPerspective(60, (float)width/(float)height, 1, 10000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            std::cout << width << " " << height << std::endl;

            ///////

            Graphics::clear();
            _myShader->start();
            _myShader->parameter(_myNoiseScaleParameter, (time/4.0f) - (int) (time/4.0f) ); 
            
            Graphics::rect(-width/2, -height/2, width, height);
            _myShader->end();

            //
            glfwSwapBuffers();
            //if (++frame % 100 == 0) 
            
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
            
            glPopMatrix();
        }
};


int main() {
    try {
        FragmentNoiseDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}


