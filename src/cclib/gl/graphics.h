#ifndef __CCLIB_GRAPHICS_INCLUDED__
#define __CCLIB_GRAPHICS_INCLUDED__

#include <cclib.h>

namespace cclib {

class Graphics {
    public:
        static void clearColor(float r, float g, float b, float a = 1.0f);
        static void clearColor(float theGray, float theAlpha=1.0f);
        static void clear();
        static void blend();
        static void noBlend();
        static void textureCoords(int theUnit, float theX, float theY, float theZ);
        static void textureCoords(int theUnit, Vector3fPtr v);
        static void vertex(float x, float y);
        static void beginShape(GLenum type); 
        static void endShape(); 
};
}; // namespace 

#endif 

