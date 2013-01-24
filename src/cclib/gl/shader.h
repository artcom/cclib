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

#include <math/vec2.h>
#include <math/vec3.h>

namespace cclib {

class Shader {

    public:
        typedef std::tr1::shared_ptr<Shader> Ptr;
        static Ptr createShader(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
                const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main"); 
        
        static Ptr createShader(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
                const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main"); 
        
        
        virtual ~Shader() {};
        static std::string combineSources(const std::vector<std::string> & shaders);

        void load();
        void start();
        void end();
        
        void destroy();
        void finalize();

        CGprogram vertexProgram();
        CGprogram fragmentProgram();
        
        CGparameter vertexParameter(const std::string & name);
        CGparameter fragmentParameter(const std::string & name);

        CGparameter createFragmentParameter(const std::string & typestring);
        CGparameter createVertexParameter(const std::string & typestring);
	
        void parameter(const CGparameter & parameter, const int & value);
        void parameter(const CGparameter & parameter, const float & value);
        void parameter(const CGparameter & parameter, const float & v1, const float & v2);
        void parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3);
        void parameter(const CGparameter & parameter, const float & v1, const float & v2, const float & v3, const float & v4);
        void parameter(const CGparameter & parameter, const Vector3f & vector);
        void parameter(const CGparameter & parameter, const bool & value);
        void parameter(const CGparameter & parameter, const Vector2f & vector);
        
        void texture(const CGparameter parameter, int textureID);
        
    protected:
        Shader(const std::vector<std::string> & vertexShaderFiles, 
                const std::vector<std::string> & fragmentShaderFiles,
                const std::string & vertexEntry = "", 
                const std::string & fragmentEntry = "" );

   
    private:
        void initShader();
        void loadVertexShader(const std::vector<std::string> & vertexPrograms);
        void loadFragmentShader(const std::vector<std::string> & fragmentPrograms);
        CGprogram loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & fragmentPrograms);
        
        void checkError(const std::string & message);

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

        std::vector<CGparameter> _usedTextureParameters;
};

}; // namespace

#endif // includeguard
