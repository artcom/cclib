#include <GL/glfw.h>

struct GL_Fixture {
    GL_Fixture() : running(true) 
    { 
        if( !glfwInit() )
        {
            running = false;
        }

        // Open an OpenGL window
        if( !glfwOpenWindow( 300, 300, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
        {
            BOOST_TEST_MESSAGE("could not open GLFW Window");
            glfwTerminate();
            running = false;
        }
        
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            BOOST_TEST_MESSAGE(glewGetErrorString(err));
            running = false;
        }
    }

    ~GL_Fixture() { 
        glfwTerminate();
    }

    void clearAndSwap() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glfwSwapBuffers();
    }

    bool running;
};

