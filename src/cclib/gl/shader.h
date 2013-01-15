// package cc.creativecomputing.graphics.shader;
// 
// import javax.media.opengl.GL;
// import javax.media.opengl.glu.GLU;
// 
// import cc.creativecomputing.graphics.CCGraphics;
// import cc.creativecomputing.io.CCIOUtil;

#ifndef __CCLIB_SHADER_INCLUDED__
#define __CCLIB_SHADER_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>
#include <Cg/cg.h>		// Cg Header
#include <Cg/cgGL.h>

namespace cclib {

class Shader {

    public:
        typedef std::tr1::shared_ptr<Shader> Ptr;
        static Ptr createShader(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
                const std::string & vertexEntry = "", const std::string & fragmentEntry = ""); 
        
        static Ptr createShader(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
                const std::string & vertexEntry = "", const std::string & fragmentEntry = ""); 
        
        
        virtual ~Shader() {};
   
    private:
        void initShader();
        void loadVertexShader(const std::vector<std::string> & vertexPrograms);
        void loadFragmentShader(const std::vector<std::string> & fragmentPrograms);
        CGprogram loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & fragmentPrograms);
        
        static std::string combineSources(const std::vector<std::string> & shaders);

        void checkError(const std::string & message);

        Shader(const std::vector<std::string> & vertexShaderFiles, 
                const std::vector<std::string> & fragmentShaderFiles,
                const std::string & vertexEntry = "", 
                const std::string & fragmentEntry = "" );

        bool _isVertexShaderEnabled, _isVertexShaderSupported;
        bool _isFragmentShaderEnabled, _isFragmentShaderSupported;
        

        std::string _vertexEntry;
        std::string _fragmentEntry;
        
        static CGcontext cg_context;
        static CGprofile cg_vertex_profile;
	    static CGprofile cg_fragment_profile;
	    
        CGprogram _vertexProgram;
	    CGprogram _fragmentProgram;
        // std::vector<CCCGMatrix> _myMatrices = new ArrayList<CCCGMatrix>();
        // std::vectorList<CCCGShaderListener> _myListener = new ArrayList<CCCGShaderListener>();
	    // Set<CGparameter> _myUsedTextureParameters = new HashSet<CGparameter>();

};

#if 0

public CCShader(
		final String[]theVertexShaderFile, 
		final String theVertexEntry, final String[] theFragmentShaderFile, 
		final String theFragmentEntry
	) {
		GL gl = CCGraphics.currentGL();
		String extensions = gl.glGetString(GL.GL_EXTENSIONS);
		
		_myIsVertexShaderSupported = extensions.indexOf("GL_ARB_vertex_shader") != -1;
		_myIsFragmentShaderSupported = extensions.indexOf("GL_ARB_fragment_shader") != -1;
		
		_myVertexEntry = theVertexEntry;
		_myFragmentEntry = theFragmentEntry;
		
		initShader();
		
		if(theVertexShaderFile != null && theVertexShaderFile[0] != null)loadVertexShader(theVertexShaderFile);
		if(theFragmentShaderFile != null && theFragmentShaderFile[0] != null)loadFragmentShader(theFragmentShaderFile);
	}
	/Users/sebastianh/artcom-git/aramco/cclib/src/cclib/stringified_shaders/sort/distancesort.fp.cpp
	/**
	 * Takes the given files and merges them to one String. 
	 * This method is used to combine the different shader sources and get rid of the includes
	 * inside the shader files.
	 * @param theFiles
	 * @return
	 */
	protected String buildSource(final String...theFiles) {
		StringBuffer myBuffer = new StringBuffer();
		
		for(String myFile:theFiles) {
			for(String myLine:CCIOUtil.loadStrings(myFile)) {
				myBuffer.append(myLine);
				myBuffer.append("\n");
			}
		}
		
		return myBuffer.toString();
	}
	
	/**
	 * Overwrite this method for initialization steps that have to be done
	 * before loading and binding the shaders
	 */
	public abstract void initShader();

	public abstract void loadVertexShader(final String...theFiles);

	public abstract void loadFragmentShader(final String...theFile);

	public abstract void load();

	public abstract void start();

	public abstract void end();
}

#endif // 0 

}; // namespace

#endif // includeguard
