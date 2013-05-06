
#include "cgshader.h"

using namespace cclib;

// initialize static variables
CGcontext CGShader::cg_context = 0;
CGprofile CGShader::cg_vertex_profile; // XXX is there a default or zero profile?
CGprofile CGShader::cg_fragment_profile;

CGShaderPtr 
CGShader::create(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
        const std::string & vertexEntry, const std::string & fragmentEntry)  
{
    std::vector<std::string> vertexShaderFiles   = std::vector<std::string>(1, vertexShaderFile);
    std::vector<std::string> fragmentShaderFiles = std::vector<std::string>(1, fragmentShaderFile);
    
    CGShaderPtr shader = CGShaderPtr(new CGShader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

CGShaderPtr
CGShader::create(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
        const std::string & vertexEntry, const std::string & fragmentEntry) 
{
    CGShaderPtr shader = CGShaderPtr(new CGShader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry));
    return shader;
}

CGShader::CGShader()
    : Shader(),
   _vertexProgram(0), _fragmentProgram(0), _usedTextureParameters()
{
    
}

CGShader::CGShader(const std::vector<std::string> & vertexShaderFiles,
        const std::vector<std::string> & fragmentShaderFiles,
        const std::string & vertexEntry, const std::string & fragmentEntry) :
   Shader(vertexShaderFiles, fragmentShaderFiles, vertexEntry, fragmentEntry),
   _vertexProgram(0), _fragmentProgram(0), _usedTextureParameters()
{
    init(vertexShaderFiles, fragmentShaderFiles);
    
//    initShader();
//
//    if (!vertexShaderFiles.empty()) {
//        loadVertexShader(vertexShaderFiles);
//    }
//    
//    if (!fragmentShaderFiles.empty()) {
//        loadFragmentShader(fragmentShaderFiles);
//    }
    
    // from CCCGShader
    // if(_myVertexProgram != null) {
    //     CGparameter myModelViewProjParameter = CgGL.cgGetNamedParameter(_myVertexProgram, theMatrixParameter);
    //     if(myModelViewProjParameter != null) {
    //         _myMatrices.add(new CCCGMatrix(myModelViewProjParameter, CCCGMatrixType.PROJECTION, CCCGMatrixTransform.IDENTITY));
    //     }
    // }
}

void
CGShader::init( const std::vector<std::string> & vertexShaderFiles,
          const std::vector<std::string> & fragmentShaderFiles)
{
    initShader();
    
    if (!vertexShaderFiles.empty()) {
        loadVertexShader(vertexShaderFiles);
    }
    
    if (!fragmentShaderFiles.empty()) {
        loadFragmentShader(fragmentShaderFiles);
    }
}

void
CGShader::initShader() {
    if(!cg_context){
        cg_context = cgCreateContext();

        checkCGError("Could not create CG Shader. Failed To Create Cg Context");

        // get the latest available vertex profile
        cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX); 

        // check if we got a valid profile 
        checkCGError("Could not create CG shader. Invalid vertex profile type");

        // get the latest available vertex profile
        cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT); 

        // check if we got a valid profile 
        checkCGError("Could not create CG shader. Invalid fragment profile type");
    }
}

void
CGShader::loadFragmentShader(const std::vector<std::string> & fragmentPrograms) {
    if(fragmentPrograms.empty()) {
        return;
    }
	
    _fragmentProgram = loadShader(_fragmentEntry, cg_fragment_profile, fragmentPrograms);
}

void
CGShader::loadVertexShader(const std::vector<std::string> & vertexPrograms) {
    if(vertexPrograms.empty()) {
        return;
    }
	
    _vertexProgram = loadShader(_vertexEntry, cg_vertex_profile, vertexPrograms);
}

CGprogram
CGShader::loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & programs) {
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

    checkCGError("Cg error(s)");

    return program;

}

void 
CGShader::checkCGError(const std::string & message) {
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
CGShader::load() {
    if(_fragmentProgram) { //  != NULL) {
        cgGLLoadProgram(_fragmentProgram); 
        checkCGError("loading fragment program");
    }
    
    if(_vertexProgram) { //  != NULL) {
        cgGLLoadProgram(_vertexProgram); 
        checkCGError("loading vertex program");
    }
    
    checkCGError("load");
}
	
void 
CGShader::start() {
    if (_vertexProgram) {
        //activate vertex shader profile
        cgGLEnableProfile(cg_vertex_profile);
        checkCGError("enabling vertex profile");

        // bind the shader program 
        cgGLBindProgram(_vertexProgram); 
        checkCGError("binding vertex program");
    }

    if(_fragmentProgram) {
        //activate fragment shader profile
        cgGLEnableProfile(cg_fragment_profile);
        checkCGError("enabling fragment profile");

        // bind the shader program 
        cgGLBindProgram(_fragmentProgram); 
        checkCGError("binding fragment program");
    }

    std::set<CGparameter>::iterator it;
    for (it = _usedTextureParameters.begin(); it!=_usedTextureParameters.end(); ++it) {
        cgGLEnableTextureParameter(*(it));
        
        checkCGError("disable texture paramters");
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
CGShader::texture(const CGparameter & parameter, int textureID) {
    cgGLSetTextureParameter(parameter, textureID);
    checkCGError("Problem setting texture ");
    
    _usedTextureParameters.insert(parameter);
}

void 
CGShader::end() {
    std::set<CGparameter>::iterator it;
    for (it = _usedTextureParameters.begin(); it!=_usedTextureParameters.end(); ++it) {
        cgGLDisableTextureParameter(*(it));
        
        checkCGError("disable texture paramters");
    }

    if(_vertexProgram) {
        cgGLDisableProfile(cg_vertex_profile);
        checkCGError("disabling vertex profile");
    }

    if(_fragmentProgram) {
        cgGLDisableProfile(cg_fragment_profile);
        checkCGError("disabling fragment profile");
    }

#if 0 // XXX do we need listeners?
    for(CCCGShaderListener myListener:_myListener) {
        myListener.onEnd();
    }
#endif
}
	
CGprogram 
CGShader::vertexProgram() {
    return _vertexProgram;
}
	
CGprogram 
CGShader::fragmentProgram() {
    return _fragmentProgram;
}

void 
CGShader::destroy(){
    cgDestroyContext(cg_context);
}
	
void
CGShader::finalize(){
    if(_vertexProgram) {
        cgDestroyProgram(_vertexProgram);
    }
    if(_fragmentProgram) {
        cgDestroyProgram(_fragmentProgram);
    }
}
	
CGparameter 
CGShader::vertexParameter(const std::string & name) {
    CGparameter myResult = cgGetNamedParameter(_vertexProgram, name.c_str());
    checkCGError("could not get vertex parameter");
    return myResult;
}

CGparameter 
CGShader::fragmentParameter(const std::string & name) {
    CGparameter result = cgGetNamedParameter(_fragmentProgram, name.c_str());
    
    checkCGError("could not get fragment parameter: " + name + " : ");
    return result;
}
	
CGparameter 
CGShader::createFragmentParameter(const std::string & typestring) {
    CGtype type = cgGetNamedUserType(fragmentProgram() /*.getBuffer()*/, typestring.c_str());
    return cgCreateParameter(cg_context, type);
}

CGparameter 
CGShader::createVertexParameter(const std::string & typestring) {
    CGtype type = cgGetNamedUserType(vertexProgram() /*.getBuffer()*/, typestring.c_str());
    return cgCreateParameter(cg_context, type);
}
	
void 
CGShader::parameter(const CGparameter & parameter, const int & value){
    cgSetParameter1i(parameter, value);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const float & value){
    cgSetParameter1f(parameter, value);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const float & v1, const float & v2){
    cgSetParameter2f(parameter, v1, v2);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3){
    cgSetParameter3f(parameter, v1, v2, v3);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3, const float & v4){
    cgSetParameter4f(parameter, v1, v2, v3, v4);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const Vector3f & vector){
    cgSetParameter3f(parameter, vector.x(), vector.y(), vector.z());
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const bool & value){
    cgSetParameter1i(parameter, value?1:0);
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter(const CGparameter & parameter, const Vector2f & vector){
    cgSetParameter2f(parameter, vector.x(), vector.y());
    checkCGError("Problem setting parameters ");
}

void 
CGShader::parameter1(const CGparameter & parameter, const std::vector<float> & values) {
    cgGLSetParameterArray1f(parameter, 0, values.size(), &(values[0]));
}

void
CGShader::parameter2(const CGparameter & parameter, const std::vector<cclib::Vector2f*> & values)
{
    std::vector<float> values_;
    for(int i=0; i<values.size(); i++)
    {
        values_.push_back(values[i]->x());
        values_.push_back(values[i]->y());
    }
    
//    printf("%s\n\t%d -> %d\n",__PRETTY_FUNCTION__,values.size(),values_.size());
    
    cgGLSetParameterArray2f(parameter, 0, values.size(), (float*)&(values_[0]));
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
		checkCGError("Problem setting parameters ");
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
