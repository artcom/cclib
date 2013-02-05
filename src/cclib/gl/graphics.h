#ifndef __CCLIB_GRAPHICS_INCLUDED__
#define __CCLIB_GRAPHICS_INCLUDED__

#include <cclib.h>

namespace cclib {

class Graphics {
    public:
        static void clearColor(float r, float g, float b, float a = 1.0f);
        static void clearColor(float theGray, float theAlpha=1.0f);
        static void color(float r, float g, float b, float a = 1.0f);
        static void rect(float x, float y, float w, float h);
        static void image(TexturePtr theTexture, float x, float y, float w, float h);
        static void clear();
        static void blend();
        static void noBlend();
        static void textureCoords(int theUnit, float theX, float theY, float theZ);
        static void textureCoords(int theUnit, Vector3fPtr v);
        static void vertex(float x, float y);
        static void vertex(float theX, float theY, float theU, float theV);
        static void beginShape(GLenum type); 
        static void endShape(); 
        static void texture(TexturePtr theTexture);
        static void noTexture();
        static void imageImplementation(TexturePtr theImage, 
                float x1, float y1, float x2, float y2,float u1, float v1, float u2, float v2);
        static void noDepthTest();
        static void blend(GLenum theBlendMode) ;

        static std::vector<TexturePtr> _myTextures; 
        static bool _myDrawTexture;
};
}; // namespace 

#endif 

