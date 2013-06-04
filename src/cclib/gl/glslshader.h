#ifndef __CCLIB_GLSLSHADER_INCLUDED__
#define __CCLIB_GLSLSHADER_INCLUDED__

#include <cclib.h>
#include <gl/shader.h>
#include <set>
#include <stdlib.h>

namespace cclib {
    
    class GLSLShader : public Shader
    {     
    public:
        // factory
        static GLSLShaderPtr create(const std::string & vertexShaderFile, const std::string fragmentShaderFile,
                                  const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main");
        
        static GLSLShaderPtr create(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles,
                                  const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main");
        
//        GLSLShader(const std::string & vertexShaderFile, const std::string fragmentShaderFile,
//                 const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main");
        
        GLSLShader( const std::vector<std::string> & vertexShaderFiles,
                 const std::vector<std::string> & fragmentShaderFiles,
                 const std::string & vertexEntry = "",
                 const std::string & fragmentEntry = "" );
        
        virtual ~GLSLShader();
        
//        void load();
        void start();
        void end();
        
        void setUniform1i(const std::string& name, int value);
        void setUniform1f(const std::string& name, float value);
        void setUniformMatrix4fv(const std::string& name, const std::vector<float>& matrix);
        void setUniformMatrix4fv(const std::string& name, const float* matrix);
        
    private:
        GLuint _program;
        
        void initShader();
        void loadVertexShader(const std::vector<std::string> & vertexPrograms);
        void loadFragmentShader(const std::vector<std::string> & fragmentPrograms);
        bool loadShader(const std::string & source, GLenum shaderType);
    };
    
}; // namespace

#endif // includeguard
