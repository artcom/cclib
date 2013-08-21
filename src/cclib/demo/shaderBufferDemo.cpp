#include <cclib.h>
#include <GL/glfw.h>

#include <gl/renderbuffer.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>

using namespace cclib;

class ShaderBufferDemo {

    private:    
	    ShaderBufferPtr _myShaderBuffer;
	    ShaderBufferPtr _myShaderBuffer2D;
        RenderBufferPtr _myRenderBuffer;
        Texture2DPtr _myTexture;
    
        int frame;

    public:
        bool running;
        
        ShaderBufferDemo() :
            _myShaderBuffer(),
            _myShaderBuffer2D(),
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
            _myShaderBuffer = ShaderBuffer::create(400, 400);
            _myShaderBuffer2D = ShaderBuffer::create(400, 400, 32, 3, 1, GL_TEXTURE_2D);
        
            cclib::TextureAttributesPtr myTextureAttributes = cclib::TextureAttributes::create(16, 4);
            cclib::FrameBufferObjectAttributesPtr myAttributes = cclib::FrameBufferObjectAttributes::create(myTextureAttributes, 1);
            _myRenderBuffer = RenderBuffer::create(myAttributes, 400, 400);

            int size=128;
            std::vector<unsigned char> data;
            for (unsigned int i=0; i<size*size; i++) {
                data.push_back(255);
                data.push_back(128);
                if (i<=64*64) {
                    data.push_back(0);
                } else {
                    data.push_back(255);
                }
                data.push_back(255);
            }
            
            _myTexture = Texture2D::create(data, GL_TEXTURE_RECTANGLE, size, size );
            _myTexture->data(data);
        }

        ~ShaderBufferDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            Graphics::clearColor(0);
            Graphics::clear();

            // 1. draw into shaderbuffer GL_TEXTURE_RECTANGLE
            _myShaderBuffer->beginDraw();
            Graphics::clearColor(1.0f, 1.0f, 0.0f, 1.0f);
            Graphics::clear();
            Graphics::color(1.0f, 0.0f, 0.0f, 1.0f); // drawing red into the texture
            Graphics::rect(0, 0, 200, 200);
            _myShaderBuffer->endDraw();
            
            // 2. draw into shaderbuffer GL_TEXTURE_2D
            _myShaderBuffer2D->beginDraw();
            Graphics::clearColor(1.0f, 1.0f, 0.0f, 1.0f);
            Graphics::clear();
            Graphics::color(1.0f, 0.0f, 0.0f, 1.0f); // drawing red into the texture
            Graphics::rect(0, 0, 200, 200);
            _myShaderBuffer2D->endDraw();
            
            // 3. draw into shaderbuffer GL_TEXTURE_2D
            _myRenderBuffer->beginDraw();
            Graphics::clearColor(1.0f, 1.0f, 0.0f, 1.0f);
            Graphics::clear();
            Graphics::color(1.0f, 0.0f, 0.0f, 1.0f); // drawing red into the texture
            Graphics::rect(0, 0, 200, 200);
            _myRenderBuffer->endDraw();
            
            // draw contents of render/shaderbuffers to screen
            Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
            Graphics::image(_myShaderBuffer->attachment(0), -0.5, 0.5, 0.5, 0.5);
            Graphics::image(_myTexture, 0, 0.5, 0.5, 0.5);
            Graphics::image(_myShaderBuffer2D->attachment(0), 0.5, 0.5, 0.5, 0.5);
            Graphics::image(_myRenderBuffer->attachment(0), -0.5, 0.0, 0.5, 0.5);
            
            ///
            glfwSwapBuffers();
            if (++frame % 100 == 0) {
                std::cout << "frame: " << frame << std::endl;
            }
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
        }
};


int main() {
    try {
        ShaderBufferDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
