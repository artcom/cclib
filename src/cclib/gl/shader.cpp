
#include "shader.h"
   
using namespace cclib;

Shader::Shader(const std::vector<std::string> & vertexShaderFiles, 
               const std::vector<std::string> & fragmentShaderFiles, 
               const std::string & vertexEntry, 
               const std::string & fragmentEntry) :
    _vertexEntry(vertexEntry), _fragmentEntry(fragmentEntry) 
{
    if (!glewIsSupported("GL_ARB_vertex_shader") || !glewIsSupported("GL_ARB_fragment_shader")) {
        throw cclib::Exception("Shaders are not supported by your hardware.");
    }
}

std::string 
Shader::combineSources(const std::vector<std::string> & shaders) 
{
    std::string result = "";
    for (std::vector<std::string>::size_type i=0; i<shaders.size(); i++) {
        result += shaders[i];
        result += "\n";
    }

    return result;
}


