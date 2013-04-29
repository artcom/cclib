
#ifndef __CCLIB_SHADER_INCLUDED__
#define __CCLIB_SHADER_INCLUDED__

#include <cclib.h>
#include <iostream>
#include <string>

// shader base class
namespace cclib {

    static void checkGLError(char *file, int line)
    {
        GLenum glErr;
        std::ostringstream errorString;
        errorString.clear();
        
        glErr = glGetError();
        while (glErr != GL_NO_ERROR)
        {
            const GLubyte* sError = gluErrorString(glErr);
            
            if (sError)
                errorString << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << std::endl;
            else
                errorString << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << std::endl;
        
            glErr = glGetError();
        }
        
        if(errorString.str().length() > 0)
        {
            throw cclib::Exception(errorString.str());
        }
    }
    
#define CHECK_GL_ERROR checkGLError((char*)__FILE__, __LINE__)
    
class Shader {
    protected:
        std::string _vertexEntry;
        std::string _fragmentEntry;

        Shader(const std::vector<std::string> & vertexShaderFiles, 
               const std::vector<std::string> & fragmentShaderFiles, 
               const std::string & vertexEntry = "main", 
               const std::string & fragmentEntry = "main");
    
    public:   
        virtual ~Shader() {};
        
        static std::string combineSources(const std::vector<std::string> & shaders);

        // virtual void start() = 0;
        // virtual void end() = 0;

        // virtual void destroy() = 0;
        // virtual void finalize() = 0;
    
        // virtual void load() = 0;
        // virtual void loadVertexShader(const std::vector<std::string> & vertexPrograms) = 0;
        // virtual void loadFragmentShader(const std::vector<std::string> & fragmentPrograms) = 0;
    
};
} // namespace

#endif

