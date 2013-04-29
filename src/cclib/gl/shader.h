
#ifndef __CCLIB_SHADER_INCLUDED__
#define __CCLIB_SHADER_INCLUDED__

#include <cclib.h>

// shader base class
namespace cclib {

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

