
#include "shader.h"
#include "../Exception.h"

using namespace cclib;

// initialize static variables
CGcontext Shader::cg_context = 0;
CGprofile Shader::cg_vertex_profile; // XXX is there a default or zero profile?
CGprofile Shader::cg_fragment_profile;

Shader::Ptr 
Shader::createShader(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
        const std::string & vertexEntry, const std::string & fragmentEntry) 
{
    std::vector<std::string> vertexShaderFiles   = std::vector<std::string>(1, vertexShaderFile);
    std::vector<std::string> fragmentShaderFiles = std::vector<std::string>(1, fragmentShaderFile);
    
    Shader::Ptr shader = Shader::Ptr(new Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

Shader::Ptr 
Shader::createShader(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
        const std::string & vertexEntry, const std::string & fragmentEntry) 
{
    Shader::Ptr shader = Shader::Ptr(new Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}
    
Shader::Shader(const std::vector<std::string> & vertexShaderFiles, 
        const std::vector<std::string> & fragmentShaderFiles,
        const std::string & vertexEntry, const std::string & fragmentEntry)
: _isVertexShaderSupported(false), _isFragmentShaderSupported(false),
  _vertexEntry(), _fragmentEntry()
{   
    _isVertexShaderSupported = glewIsSupported("GL_ARB_vertex_shader"); 
    _isFragmentShaderSupported = glewIsSupported("GL_ARB_fragment_shader");
    
    if (!_isFragmentShaderSupported || !_isFragmentShaderSupported) {
        throw Exception("Shaders are not supported by your hardware.");
    }
    
    _vertexEntry = vertexEntry;
    _fragmentEntry = fragmentEntry;
    initShader();

    if (!vertexShaderFiles.empty()) {
        loadVertexShader(vertexShaderFiles);
    }
    
    if (!fragmentShaderFiles.empty()) {
        loadVertexShader(fragmentShaderFiles);
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

void 
Shader::initShader() {
    if(!cg_context){
        cg_context = cgCreateContext();

        checkError("Could not create CG Shader. Failed To Create Cg Context");

        // get the latest available vertex profile
        cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX); 

        // check if we got a valid profile 
        checkError("Could not create CG shader. Invalid vertex profile type");

        // get the latest available vertex profile
        cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT); 

        // check if we got a valid profile 
        checkError("Could not create CG shader. Invalid fragment profile type");
    }
}

void
Shader::loadFragmentShader(const std::vector<std::string> & fragmentPrograms) {
    if(fragmentPrograms.empty()) {
        return;
    }
	
    _fragmentProgram = loadShader(_fragmentEntry, cg_fragment_profile, fragmentPrograms);
}

void
Shader::loadVertexShader(const std::vector<std::string> & vertexPrograms) {
    if(vertexPrograms.empty()) {
        return;
    }
	
    _vertexProgram = loadShader(_vertexEntry, cg_vertex_profile, vertexPrograms);
}

CGprogram
Shader::loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & programs) {
    // set the current Profile
#warning "check that!"
    // cgGLSetContextOptimalOptions(cg_context, profile);
    cgSetAutoCompile(cg_context, CG_COMPILE_MANUAL);

    std::string shaderSource = combineSources(programs);

    CGprogram program = cgCreateProgram(
            cg_context, 
            CG_SOURCE,
            shaderSource.c_str(), 
            profile, 
            entry.c_str(), 
            0);

    checkError("Cg error(s)");

    return program;

}

void 
Shader::checkError(const std::string & message) {
    CGerror error = cgGetError();

    if(error!= CG_NO_ERROR){
        std::string errorString = std::string(message); 

        if(error == CG_COMPILER_ERROR) {
            errorString = errorString + cgGetLastListing(cg_context);
        }
        
        throw new Exception(errorString + std::string("\n ") + std::string(cgGetErrorString(error)));
    }		
}
