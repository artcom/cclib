
#include "graphics.h"
#include <math/vec2.h>
#include <math/vec3.h>

using namespace cclib;

void 
Graphics::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
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
Graphics::blend() {
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_ADD, GL_ADD);
}

void 
Graphics::noBlend() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
	glDisable(GL_BLEND);
}

void 
Graphics::textureCoords(int theUnit, float theX, float theY, float theZ) {
	glMultiTexCoord3f(theUnit, theX, theY, theZ);
}

void 
Graphics::textureCoords(int theUnit, Vector3fPtr v) {
	glMultiTexCoord3f(theUnit, v->x(), v->y(), v->z());
}

void 
Graphics::vertex(float x, float y) {
    glVertex2f(x, y);
}

void 
Graphics::beginShape(GLenum type) {
    glBegin(type);
}

void 
Graphics::endShape(){
    glEnd();
}


