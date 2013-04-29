#ifndef __CCLIB_CGSHADER_INCLUDED__
#define __CCLIB_CGSHADER_INCLUDED__

#include <cclib.h>
#include <gl/shader.h>
#include <set>

namespace cclib {

class CGShader : public Shader {

    public:
        // factory 
        static CGShaderPtr create(const std::string & vertexShaderFile, const std::string fragmentShaderFile, 
                const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main"); 
        
        static CGShaderPtr create(const std::vector<std::string> & vertexShaderFiles, const std::vector<std::string> & fragmentShaderFiles, 
                const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main"); 
        
        CGShader(const std::string & vertexShaderFile, const std::string fragmentShaderFile,
               const std::string & vertexEntry = "main", const std::string & fragmentEntry = "main");
    
        virtual ~CGShader() {};

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
        void parameter1(const CGparameter & parameter, const std::vector<float> & values);
	
        void texture(const CGparameter & parameter, int textureID);
        void checkError(const std::string & message);
        
    protected:
        CGShader( const std::vector<std::string> & vertexShaderFiles, 
                  const std::vector<std::string> & fragmentShaderFiles,
                  const std::string & vertexEntry = "", 
                  const std::string & fragmentEntry = "" );
   
    private:
        void initShader();
        void loadVertexShader(const std::vector<std::string> & vertexPrograms);
        void loadFragmentShader(const std::vector<std::string> & fragmentPrograms);
        CGprogram loadShader(const std::string & entry, CGprofile profile, const std::vector<std::string> & fragmentPrograms);
        
        static CGcontext cg_context;
        static CGprofile cg_vertex_profile;
	    static CGprofile cg_fragment_profile;
	    
        CGprogram _vertexProgram;
	    CGprogram _fragmentProgram;
        // std::vector<CCCGMatrix> _myMatrices = new ArrayList<CCCGMatrix>();
        // std::vectorList<CCCGShaderListener> _myListener = new ArrayList<CCCGShaderListener>();
	    // Set<CGparameter> _myUsedTextureParameters = new HashSet<CGparameter>();

        std::set<CGparameter> _usedTextureParameters;
};

}; // namespace

#endif // includeguard
