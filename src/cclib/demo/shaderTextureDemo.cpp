#include <cclib.h>
#include <GL/glfw.h>

#include <gl/shadertexture.h>
#include <gl/graphics.h>

using namespace cclib;

class ShaderTextureDemo {

    private:    
	    ShaderTexturePtr _myShaderTexture;
    
        int frame;

    public:
        bool running;
        
        ShaderTextureDemo() :
            _myShaderTexture(),
            frame(0), running(true)
        { 
            if(!glfwInit()) {
                running = false;
            }

            // Open an OpenGL window
            if(!glfwOpenWindow( 500, 500, 0, 0, 0, 0, 0, 0, GLFW_WINDOW )) {
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
            _myShaderTexture = ShaderTexture::create(400, 400);
        }

        ~ShaderTextureDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            Graphics::clearColor(0);
            Graphics::clear();
            _myShaderTexture->beginDraw();
            
            Graphics::clearColor(1.0f, 0.0f, 0.0f, 1.0f);
            Graphics::clear();
            Graphics::color(1.0f, 0.0f, 0.0f, 1.0f); // drawing red into the texture
            Graphics::rect(-200, -200, 50, 50);
            
            _myShaderTexture->endDraw();
                    
            Graphics::color(1.0f, 1.0f, 0.0f, 1.0f); // yellow. should not be visible 
            Graphics::image(_myShaderTexture, -0.5, 0.5, 0.5, 0.5);
//         
            glfwSwapBuffers();
            if (++frame % 100 == 0) {
                std::cout << "frame: " << frame << std::endl;
            }
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
        }
};


int main() {
    try {
        ShaderTextureDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}