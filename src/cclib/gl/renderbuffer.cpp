#include <gl/renderbuffer.h>
#include <gl/graphics.h>
#include <gl/camera.h>

using namespace cclib;


RenderBufferPtr
RenderBuffer::create(FrameBufferObjectAttributesPtr attributes, int width, int height, GLenum theTarget) 
{
    return RenderBufferPtr(new RenderBuffer(attributes, width, height));
}

RenderBuffer::RenderBuffer ( FrameBufferObjectAttributesPtr attributes, unsigned int width, unsigned int height, GLenum target) :
    FrameBufferObject(target, attributes, width, height)
{
    _camera = Camera::create(width, height);
}

void 
RenderBuffer::beginDraw() {
    bindFBO();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
 
    _camera->draw();
}

void 
RenderBuffer::endDraw() {
    // XXX restore old camera: Graphics::camera().draw(_myGraphics);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); 
    releaseFBO();
}
	
CameraPtr 
RenderBuffer::camera() {
    return _camera;
}

