#ifndef __CCLIB_GPUNOISE_INCLUDED 
#define __CCLIB_GPUNOISE_INCLUDED 

#include <cclib.h>

namespace cclib {

class GPUNoise 
{
    public:
        static GPUNoisePtr get();
	    static void attachFragmentNoise(CGShader * theShader);
        static void attachVertexNoise(CGShader * theShader);
        ~GPUNoise() {};

    private:
	    static GPUNoisePtr _instance;
	    
        static int perm[256];        
        static int grad3[16][3];
        static int grad4[32][4];
        static int simplex4[64][4];
    
        GLuint permTextureID;
        GLuint simplexTextureID;
        GLuint gradTextureID;
	
        GPUNoise();

        void initPermTexture();
        void initSimplexTexture();
        void initGradTexture();
};

}; // namespace

#endif
