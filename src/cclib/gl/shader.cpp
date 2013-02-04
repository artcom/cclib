
#include "shader.h"

using namespace cclib;

// initialize static variables
CGcontext Shader::cg_context = 0;
CGprofile Shader::cg_vertex_profile; // XXX is there a default or zero profile?
CGprofile Shader::cg_fragment_profile;

ShaderPtr 
Shader::create(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
        const std::string & vertexEntry, const std::string & fragmentEntry) 
{
    std::vector<std::string> vertexShaderFiles   = std::vector<std::string>(1, vertexShaderFile);
    std::vector<std::string> fragmentShaderFiles = std::vector<std::string>(1, fragmentShaderFile);
    
    ShaderPtr shader = ShaderPtr(new Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

ShaderPtr 
Shader::create(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
        const std::string & vertexEntry, const std::string & fragmentEntry) 
{
    ShaderPtr shader = ShaderPtr(new Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}
    
Shader::Shader(const std::vector<std::string> & vertexShaderFiles, 
        const std::vector<std::string> & fragmentShaderFiles,
        const std::string & vertexEntry, const std::string & fragmentEntry) :
   _vertexEntry(vertexEntry), _fragmentEntry(fragmentEntry),
   _vertexProgram(0), _fragmentProgram(0), _usedTextureParameters()
{   
    if (!glewIsSupported("GL_ARB_vertex_shader") || !glewIsSupported("GL_ARB_fragment_shader")) {
        throw cclib::Exception("Shaders are not supported by your hardware.");
    }
    
    initShader();

    if (!vertexShaderFiles.empty()) {
        loadVertexShader(vertexShaderFiles);
    }
    
    if (!fragmentShaderFiles.empty()) {
        loadFragmentShader(fragmentShaderFiles);
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
    // std::cout << "fragment profile: " << cgGetProfileString(cgGLGetLatestProfile(CG_GL_FRAGMENT)) << std::endl; 
}

void
Shader::loadVertexShader(const std::vector<std::string> & vertexPrograms) {
    if(vertexPrograms.empty()) {
        return;
    }
	
    _vertexProgram = loadShader(_vertexEntry, cg_vertex_profile, vertexPrograms);
    // std::cout << "vertex profile: " << cgGetProfileString(cgGLGetLatestProfile(CG_GL_VERTEX)) << std::endl; 
}

CGprogram
Shader::loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & programs) {
    // set the current Profile
    cgGLSetContextOptimalOptions(cg_context, profile);
    cgSetAutoCompile(cg_context, CG_COMPILE_MANUAL);
    std::string shaderSource = combineSources(programs);

    CGprogram program = cgCreateProgram(
            cg_context, 
            CG_SOURCE,
            shaderSource.c_str(), 
            profile, 
            entry.size()==0 ? "main" : entry.c_str(), 
            0);

    checkError("Cg error(s)");

    return program;

}

void 
Shader::checkError(const std::string & message) {
    CGerror error = cgGetError();

    if(error!= CG_NO_ERROR) {
        std::string errorString = std::string(message); 

        if(error == CG_COMPILER_ERROR) {
            errorString = errorString + cgGetLastListing(cg_context);
        }
        
        throw cclib::Exception(errorString + std::string("\n ") + std::string(cgGetErrorString(error)));
    }		
}

void 
Shader::load() {
    if(_fragmentProgram) { //  != NULL) {
        cgGLLoadProgram(_fragmentProgram); 
        checkError("loading fragment program");
    }
    
    if(_vertexProgram) { //  != NULL) {
        cgGLLoadProgram(_vertexProgram); 
        checkError("loading vertex program");
    }
}
	
void 
Shader::start() {
    if (_vertexProgram) {
        //activate vertex shader profile
        cgGLEnableProfile(cg_vertex_profile);
        checkError("enabling vertex profile");

        // bind the shader program 
        cgGLBindProgram(_vertexProgram); 
        checkError("binding vertex program");
    }

    if(_fragmentProgram) {
        //activate fragment shader profile
        cgGLEnableProfile(cg_fragment_profile);
        checkError("enabling fragment profile");

        // bind the shader program 
        cgGLBindProgram(_fragmentProgram); 
        checkError("binding fragment program");
    }

// XXX untested
    for(std::vector<CGparameter>::size_type i=0; i<_usedTextureParameters.size(); i++) { 
        cgGLEnableTextureParameter(_usedTextureParameters[i]);
    }

#if 0 // XXX do we need listeners?
    for(CCCGShaderListener myListener:_myListener) {
        myListener.onStart();
    }
#endif
}
	
/**
 * Sets the texture for a given cgparameter. Be aware that you have to call
 * this before you call the start method of the shader. As all textures get
 * automatically enabled in the start method. 
 * @param parameter cg parameter
 * @param theTextureID id of the texture
 */

// XXX untested --------------------------------------------------------------------------------------
void 
Shader::texture(const CGparameter parameter, int textureID) {
    cgGLSetTextureParameter(parameter, textureID);
    checkError("Problem setting texture ");
    _usedTextureParameters.push_back(parameter);
}

void 
Shader::end() {
    for(std::vector<CGparameter>::size_type i=0; i<_usedTextureParameters.size(); i++) { 
        cgGLDisableTextureParameter(_usedTextureParameters[i]);
    }

    if(_vertexProgram) {
        cgGLDisableProfile(cg_vertex_profile);
        checkError("disabling vertex profile");
    }

    if(_fragmentProgram) {
        cgGLDisableProfile(cg_fragment_profile);
        checkError("disabling fragment profile");
    }

#if 0 // XXX do we need listeners?
    for(CCCGShaderListener myListener:_myListener) {
        myListener.onEnd();
    }
#endif
}
	
CGprogram 
Shader::vertexProgram() {
    return _vertexProgram;
}
	
CGprogram 
Shader::fragmentProgram() {
    return _fragmentProgram;
}

void 
Shader::destroy(){
    cgDestroyContext(cg_context);
}
	
void
Shader::finalize(){
    if(_vertexProgram) {
        cgDestroyProgram(_vertexProgram);
    }
    if(_fragmentProgram) {
        cgDestroyProgram(_fragmentProgram);
    }
}
	
CGparameter 
Shader::vertexParameter(const std::string & name) {
    CGparameter myResult = cgGetNamedParameter(_vertexProgram, name.c_str());
    checkError("could not get vertex parameter");
    return myResult;
}

CGparameter 
Shader::fragmentParameter(const std::string & name) {
    CGparameter result = cgGetNamedParameter(_fragmentProgram, name.c_str());
    checkError("could not get fragment parameter: " + name + " : ");
    return result;
}
	
CGparameter 
Shader::createFragmentParameter(const std::string & typestring) {
    CGtype type = cgGetNamedUserType(fragmentProgram() /*.getBuffer()*/, typestring.c_str());
    return cgCreateParameter(cg_context, type);
}

CGparameter 
Shader::createVertexParameter(const std::string & typestring) {
    CGtype type = cgGetNamedUserType(vertexProgram() /*.getBuffer()*/, typestring.c_str());
    return cgCreateParameter(cg_context, type);
}
	
void 
Shader::parameter(const CGparameter & parameter, const int & value){
    cgSetParameter1i(parameter, value);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const float & value){
    cgSetParameter1f(parameter, value);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const float & v1, const float & v2){
    cgSetParameter2f(parameter, v1, v2);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3){
    cgSetParameter3f(parameter, v1, v2, v3);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3, const float & v4){
    cgSetParameter4f(parameter, v1, v2, v3, v4);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const Vector3f & vector){
    cgSetParameter3f(parameter, vector.x(), vector.y(), vector.z());
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const bool & value){
    cgSetParameter1i(parameter, value?1:0);
    checkError("Problem setting parameters ");
}

void 
Shader::parameter(const CGparameter & parameter, const Vector2f & vector){
    cgSetParameter2f(parameter, vector.x(), vector.y());
    checkError("Problem setting parameters ");
}

void 
Shader::parameter1(const CGparameter & parameter, const std::vector<float> & values) {
    cgGLSetParameterArray1f(parameter, 0, values.size(), &(values[0]));
}
	
// JAVA LINE -- DO NOT CROSS!
#if 0
public void addMatrix(const String parameterName, const CCCGMatrixType theMatrixType, const CCCGMatrixTransform theMatrixTransform) {
		_myMatrices.add(new CCCGMatrix(vertexParameter(parameterName),theMatrixType, theMatrixTransform));
	}
	
	public void addFragmentMatrix(const String parameterName, const CCCGMatrixType theMatrixType, const CCCGMatrixTransform theMatrixTransform) {
		_myMatrices.add(new CCCGMatrix(fragmentParameter(parameterName),theMatrixType, theMatrixTransform));
	}
	
	public void addListener(const CCCGShaderListener theListener) {
		_myListener.add(theListener);
	}

	public void updateMatrices(){
		for(CCCGMatrix myMatrix:_myMatrices) {
			myMatrix.update();
		}
	}

	public void parameter(const CGparameter parameter, const CCColor theColor){
		cgSetParameter4f(parameter, theColor.r, theColor.g, theColor.b, theColor.a);
		checkError("Problem setting parameters ");
	}
        
	}
	
	public void parameter1(const CGparameter parameter, const float...parameterValues) {
		FloatBuffer myValueBuffer = FloatBuffer.wrap(parameterValues);
		myValueBuffer.flip();
		
		cgGLSetParameterArray1f(parameter, 0, parameterValues.length, myValueBuffer);
	}
	
	public void parameter2(const CGparameter parameter, const List<CCVector2f> parameterValues) {
		FloatBuffer myValueBuffer = FloatBuffer.allocate(parameterValues.size() * 2);
		for(CCVector2f myVector:parameterValues) {
			myValueBuffer.put(myVector.x);
			myValueBuffer.put(myVector.y);
		}
		myValueBuffer.flip();
		
		cgGLSetParameterArray2f(parameter, 0, parameterValues.size(), myValueBuffer);
	}
	
	public void parameter2(const CGparameter parameter, const CCVector2f[] parameterValues) {
		FloatBuffer myValueBuffer = FloatBuffer.allocate(parameterValues.length * 2);
		for(CCVector2f myVector:parameterValues) {
			myValueBuffer.put(myVector.x);
			myValueBuffer.put(myVector.y);
		}
		myValueBuffer.flip();
		
		cgGLSetParameterArray2f(parameter, 0, parameterValues.length, myValueBuffer);
	}
	
	public void parameter3(const CGparameter parameter, const List<CCVector3f> parameterValues) {
		FloatBuffer myValueBuffer = FloatBuffer.allocate(parameterValues.size() * 3);
		for(CCVector3f myVector:parameterValues) {
			myValueBuffer.put(myVector.x);
			myValueBuffer.put(myVector.y);
			myValueBuffer.put(myVector.z);
		}
		myValueBuffer.flip();
		
		cgGLSetParameterArray3f(parameter, 0, parameterValues.size(), myValueBuffer);
	}
	
	public void matrix(const CGparameter parameter, const CCMatrix4f theMatrix) {
		cgSetMatrixParameterfc(parameter, theMatrix.toFloatBuffer());
	}
	
	public void matrix(const CGparameter parameter, const CCCGMatrixType theMatrixType, const CCCGMatrixTransform theMatrixTransform) {
		cgGLSetStateMatrixParameter(parameter, theMatrixType.cgID, theMatrixTransform.cgID);
	}
#endif 
