
#include "graphics.h"
#include <math/vec2.h>
#include <math/vec3.h>
#include <gl/texture.h>

using namespace cclib;

std::vector<TexturePtr> Graphics::_myTextures = std::vector<TexturePtr>(8, TexturePtr());
bool Graphics::_myDrawTexture = false;
int Graphics::_myRectMode = CORNER;

void 
Graphics::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void 
Graphics::color(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void
Graphics::color(cclib::Color & color)
{
    glColor4f(color.red(), color.green(), color.blue(), color.alpha());
}

void 
Graphics::clearColor(float theGray, float theAlpha) {
    glClearColor(theGray, theGray, theGray, theAlpha);
}

void 
Graphics::clear() {
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void
Graphics::textureCoords(int theUnit, float theX, float theY) {
	glMultiTexCoord2f(theUnit, theX, theY);
}

void
Graphics::textureCoords(int theUnit, float theX, float theY, float theZ) {
	glMultiTexCoord3f(theUnit, theX, theY, theZ);
}

void
Graphics::textureCoords(int theUnit, float theX, float theY, float theZ, float theW) {
	glMultiTexCoord4f(theUnit, theX, theY, theZ, theW);
}

void 
Graphics::textureCoords(int theUnit, Vector3fPtr v) {
	glMultiTexCoord3f(theUnit, v->x(), v->y(), v->z());
}

void 
Graphics::textureCoords(int theUnit, Vector3f v) {
	glMultiTexCoord3f(theUnit, v.x(), v.y(), v.z());
}

void 
Graphics::vertex(float x, float y) {
    glVertex2f(x, y);
}

void 
Graphics::vertex(float theX, float theY, float theU, float theV){
    glTexCoord2f(theU, theV);  // was: Graphics::textureCoords(theU, theV);
    glVertex2f(theX, theY);
}

void 
Graphics::beginShape(GLenum type) {
    glBegin(type);
}

void 
Graphics::endShape(){
    glEnd();
}

void 
Graphics::rect( float theX1, float theY1, float theX2, float theY2 )
{
    float hradius, vradius;

    switch (_myRectMode){
        case CORNERS:
            break;
        case CORNER:
            theX2 += theX1;
            theY2 += theY1;
            break;
        case RADIUS:
            hradius = theX2;
            vradius = theY2;
            theX2 = theX1 + hradius;
            theY2 = theY1 + vradius;
            theX1 -= hradius;
            theY1 -= vradius;
            break;
        case CENTER:
            hradius = theX2 / 2.0f;
            vradius = theY2 / 2.0f;
            theX2 = theX1 + hradius;
            theY2 = theY1 + vradius;
            theX1 -= hradius;
            theY1 -= vradius;
    }

    if (theX1 > theX2){
        float temp = theX1;
        theX1 = theX2;
        theX2 = temp;
    }

    if (theY1 > theY2){
        float temp = theY1;
        theY1 = theY2;
        theY2 = temp;
    }

    rectImpl(theX1, theY1, theX2, theY2);
}

void 
Graphics::rectImpl(float x1, float y1, float x2, float y2) {
    quad(x1, y2,x2, y2, x2, y1, x1, y1);
}

void 
Graphics::quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    beginShape(GL_QUADS);
    vertex(x1, y1);
    vertex(x2, y2);
    vertex(x3, y3);
    vertex(x4, y4);
    endShape();
}

void 
Graphics::image( TexturePtr theImage, float theX, float theY, 
		float theWidth, float theHeight ) 
{
    // TextureMode myStoredTextureMode = _myTextureMode;
    // _myTextureMode = CCTextureMode.IMAGE;
    Graphics::imageImplementation( theImage, theX, theY, theWidth, theHeight, 
        0, 0, theImage->width(), theImage->height());
    // _myTextureMode = myStoredTextureMode;
}

void 
Graphics::imageImplementation(TexturePtr theImage,
		float x1, float y1, float x2, float y2,
		float u1, float v1, float u2, float v2)
{
    //		switch(_myImageMode){
    //		case CENTER:
    //			// reset a negative width
    //			if (x2 < 0){ x1 += x2; x2 = -x2;}
    //			// reset a negative height
    //			if (y2 < 0){ y1 += y2; y2 = -y2;}
    //			x1 -= x2/2;
    //			y1 -= y2/2;
    //			x2 += x1;
    //			y2 += y1;
    //			break;
    //		case CORNER:

    // reset a negative width
    if (x2 < 0){ x1 += x2; x2 = -x2;}
    // reset a negative height
    if (y2 < 0){ y1 += y2; y2 = -y2;}

    x2 += x1;
    y2 += y1;

    //			break;
    // 		case CORNERS:
    // 			// reverse because x2 < x1
    // //			if (x2 < x1){
    // //				float temp = x1; 
    // //				x1 = x2; 
    // //				x2 = temp;
    // //			}
    // //				
    // //			// reverse because y2 < y1
    // //			if (y2 < y1){
    // //				float temp = y1; 
    // //				y1 = y2; 
    // //				y2 = temp;
    // //			}
    // 		}
		
    Graphics::texture(theImage);

    Graphics::beginShape(GL_QUADS);
    Graphics::vertex(x1, y1, u1, v1);
    Graphics::vertex(x1, y2, u1, v2);
    Graphics::vertex(x2, y2, u2, v2);
    Graphics::vertex(x2, y1, u2, v1);
    Graphics::endShape();
    Graphics::noTexture();
}

void 
Graphics::texture(unsigned int theTextureUnit, TexturePtr theTexture) {
    // GL_TEXTURE_RECTANGLE_ARB
    
    _myTextures[theTextureUnit] = theTexture;
    glActiveTexture(GL_TEXTURE0 + theTextureUnit);
    glEnable(_myTextures[theTextureUnit]->target());

    _myTextures[theTextureUnit]->bind();
    // applyTextureTransformation(theTextureUnit, theTexture);
    glActiveTexture(GL_TEXTURE0);

    _myDrawTexture = true;
}

void 
Graphics::texture(TexturePtr theTexture) {
    _myTextures[0] = theTexture;
    glEnable( _myTextures[0]->target() );
    _myTextures[0]->bind();
    _myDrawTexture = true;

    // applyTextureTransformation(0, theTexture);
}

void
Graphics::noTexture() {
    for (int i = 0; i < _myTextures.size(); i++) {
        if (_myTextures[i]) {
            glActiveTexture(GL_TEXTURE0 + i);
            glDisable(_myTextures[i]->target());
            // _myTextures[i] = null; // getting cleared later
            // removeTextureTransformation();
        }
    
        _myTextures[i] = TexturePtr();
    }

    glActiveTexture(GL_TEXTURE0);
    _myDrawTexture = false;
}

void
Graphics::depthTest() {
    glEnable(GL_DEPTH_TEST);
}

void 
Graphics::noDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void
//Graphics::checkError()
Graphics::checkErrorX(char *file, int line)
{
    GLenum error = glGetError();
    
    std::string errorStr;
    switch(error) {
        case GL_NO_ERROR:
            return;
            break;
        case GL_INVALID_ENUM:
            errorStr = " # INVALID ENUMERATION REPORTED. check for errors in OPENGL calls with constants.";
            break;
        case GL_INVALID_VALUE:
            errorStr = "# INVALID VALUE REPORTED. check for errors with passed values that are out of a defined range.";
            break;
        case GL_INVALID_OPERATION:
            errorStr = "# INVALID OPERATION REPORTED. check for function calls that are invalid in the current graphics state.";
            break;
        case GL_STACK_OVERFLOW:
            errorStr = "# STACK OVERFLOW REPORTED. check for errors in matrix operations";
            break;
        case GL_STACK_UNDERFLOW:
            errorStr = "# STACK UNDERFLOW REPORTED. check for errors  in matrix operations";
            break;
        case GL_OUT_OF_MEMORY:
            errorStr = "# OUT OF MEMORY. not enough memory to execute the commands";
            break;
        case GL_TABLE_TOO_LARGE:
            errorStr = "# TABLE TOO LARGE.";
            break;
        default:
            errorStr = "unknown gl error";
            break;
    }
    
    std::ostringstream errorString;
    errorString << "GL check Error: " << errorStr << "\n\tin File " << file << " at line: " << line << std::endl;
    throw cclib::Exception(errorString.str());
}

