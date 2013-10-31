#ifndef __CCLIB_GRAPHICS_INCLUDED__
#define __CCLIB_GRAPHICS_INCLUDED__

#include <cclib.h>
#include "gl/texture2d.h"

namespace cclib {

#define BLEND_MODE_ADD   0x3 // ADD(CCBlendFactor.SRC_ALPHA, CCBlendFactor.ONE, CCBlendEquation.ADD),
#define BLEND_MODE_BLEND 0x4 // BLEND(CCBlendFactor.SRC_ALPHA, CCBlendFactor.ONE_MINUS_SRC_ALPHA, CCBlendEquation.ADD),

#define CORNERS 1
#define CORNER  2
#define RADIUS  3
#define CENTER  4

//#define DEFAULT_FOV 60
#define DEFAULT_FOV 20

class Graphics {
    public:
        static void clearColor(float r, float g, float b, float a = 1.0f);
        static void clearColor(float theGray, float theAlpha=1.0f);
        static void color(float r, float g, float b, float a = 1.0f);
        static void color(cclib::Color & color);
        static void image(TexturePtr theTexture, float x, float y, float w, float h);
        static void clear();
        static void textureCoords(int theUnit, float theX, float theY);
        static void textureCoords(int theUnit, float theX, float theY, float theZ);
        static void textureCoords(int theUnit, float theX, float theY, float theZ, float theW);
        static void textureCoords(int theUnit, Vector3fPtr v);
        static void textureCoords(int theUnit, Vector3f v);
        static void vertex(float x, float y);
        static void vertex(float theX, float theY, float theU, float theV);
        static void beginShape(GLenum type);
        static void endShape();
        static void texture(unsigned int theTextureUnit, TexturePtr theTexture);
        static void texture(TexturePtr theTexture);
        static void noTexture();
        static void imageImplementation(TexturePtr theImage,
                float x1, float y1, float x2, float y2,float u1, float v1, float u2, float v2);
        static void depthTest();
        static void noDepthTest();
//        static void checkError();

        static void lineWidth(float theLineWidth);
        static void line(Vector3f v1, Vector3f v2);

        static void checkErrorX(char *file, int line);
#define checkError() Graphics::checkErrorX((char*)__FILE__, __LINE__)

        static bool checkFramebufferStatus();

        static void blend(unsigned int theBlendmode) {
            glEnable(GL_BLEND);
            Graphics::checkError();

            switch(theBlendmode) {
                case BLEND_MODE_ADD:
                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);
                    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                    Graphics::checkError();
                    break;
                case BLEND_MODE_BLEND:
                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                    Graphics::checkError();
                    break;
                 default:
                    std::cout << "unported blend mode";
                    break;
            };
        };

        static void blend() {
            Graphics::blend(BLEND_MODE_BLEND);
        };

        static void endBlend() {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
        };

        static void noBlend() {
            Graphics::endBlend();
            glDisable(GL_BLEND);
        }

        static void rect( float theX1, float theY1, float theX2, float theY2 );
        static void rectImpl(float x1, float y1, float x2, float y2);
        static void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

        static int _myRectMode;
        static std::vector<TexturePtr> _myTextures;
        static bool _myDrawTexture;

        static void pointSize(float thePointSize) {
            glPointSize(thePointSize);
        };

        static void strokeWeight(float theStrokeWeight) {
            glLineWidth(theStrokeWeight);
        };

        static void smooth() {
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);

            // antialiasing
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        };

        static void noSmooth() {
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            glDisable(GL_BLEND);
        };

        static void pushAttribute() {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
        };

        static void popAttribute() {
            glPopAttrib();
        };
};
}; // namespace

#endif

