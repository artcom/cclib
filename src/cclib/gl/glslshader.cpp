#include "glslshader.h"

using namespace cclib;

GLSLShaderPtr
GLSLShader::create(const std::string & vertexShaderFile, const std::string fragmentShaderFile,
                 const std::string & vertexEntry, const std::string & fragmentEntry)
{
    std::vector<std::string> vertexShaderFiles   = std::vector<std::string>(1, vertexShaderFile);
    std::vector<std::string> fragmentShaderFiles = std::vector<std::string>(1, fragmentShaderFile);
    
    GLSLShaderPtr shader = GLSLShaderPtr(new GLSLShader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

GLSLShaderPtr
GLSLShader::create(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles,
                 const std::string & vertexEntry, const std::string & fragmentEntry)
{
    GLSLShaderPtr shader = GLSLShaderPtr(new GLSLShader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

GLSLShader::GLSLShader(const std::vector<std::string> & vertexShaderFiles,
                   const std::vector<std::string> & fragmentShaderFiles,
                   const std::string & vertexEntry, const std::string & fragmentEntry) :
    Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry),
    _program(0)
{
    initShader();
    
    if (!vertexShaderFiles.empty()) {
        loadVertexShader(vertexShaderFiles);
    }
    
    if (!fragmentShaderFiles.empty()) {
        loadFragmentShader(fragmentShaderFiles);
    }
}

GLSLShader::~GLSLShader()
{
    glDeleteObjectARB(_program);
    CHECK_GL_ERROR;
}

void GLSLShader::initShader()
{
    _program = glCreateProgramObjectARB();
    CHECK_GL_ERROR;
}

void GLSLShader::loadVertexShader(const std::vector<std::string> & vertexPrograms)
{
    std::string shaderSource = combineSources(vertexPrograms);
//    printf("%s\n%s\n",__PRETTY_FUNCTION__,shaderSource.c_str());
    
    loadShader(shaderSource, GL_VERTEX_SHADER);
}

void GLSLShader::loadFragmentShader(const std::vector<std::string> & fragmentPrograms)
{
    std::string shaderSource = combineSources(fragmentPrograms);
//    printf("%s\n%s\n",__PRETTY_FUNCTION__,shaderSource.c_str());
    
    loadShader(shaderSource, GL_FRAGMENT_SHADER);
}

bool GLSLShader::loadShader(const std::string & source, GLenum shaderType)
{
    if(source.empty()) return false;
    
    GLint compiled = 0;
    const char* ptr = source.c_str();
    
    GLuint shader = glCreateShaderObjectARB(shaderType);
    CHECK_GL_ERROR;
    
    glShaderSourceARB(shader, 1, (const GLcharARB **)&ptr, NULL);
    CHECK_GL_ERROR;
    
    glCompileShaderARB(shader);
    CHECK_GL_ERROR;
    
#if 0
    glGetObjectParameterivARB(shader, GL_COMPILE_STATUS, &compiled);
    CHECK_GL_ERROR();
#else
    GLint result = 0;
    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &result);
    if(result != GL_TRUE)
    {
        int kBufferSize = 1024;
        char buffer[1024];
        
        GLsizei length = 0;
        glGetInfoLogARB(shader, kBufferSize-1,
                        &length, buffer);
        throw cclib::Exception("GLSL error\n" + std::string(buffer));
    }
#endif
    
    if (!compiled) return false;
    
    glAttachObjectARB(_program, shader);
    CHECK_GL_ERROR;
    
    glLinkProgramARB(_program);
    CHECK_GL_ERROR;
    
    return true;
}

void GLSLShader::start()
{
    glUseProgramObjectARB(_program);
}

void GLSLShader::end()
{
    glUseProgramObjectARB(0);
}

void GLSLShader::setUniform1i(const std::string& name, int value)
{
    GLint location = glGetUniformLocationARB(_program, name.c_str());
    glUniform1iARB(location, value);
    CHECK_GL_ERROR;
}

void GLSLShader::setUniform1f(const std::string& name, float value)
{
    GLint location = glGetUniformLocationARB(_program, name.c_str());
    glUniform1fARB(location, value);
    CHECK_GL_ERROR;
}