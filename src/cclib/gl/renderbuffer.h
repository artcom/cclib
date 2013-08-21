#ifndef __CCLIB_RENDERBUFFER_INCLUDED__
#define __CCLIB_RENDERBUFFER_INCLUDED__

#include <cclib.h>
#include <gl/framebufferobject.h>

namespace cclib {

class RenderBuffer : public FrameBufferObject {

    public:
        virtual ~RenderBuffer() {};
        static RenderBufferPtr create(FrameBufferObjectAttributesPtr attributes, int width, int height, GLenum theTarget = GL_TEXTURE_2D);

        void beginDraw();
        void endDraw();
        CameraPtr camera();

    private:
        RenderBuffer(FrameBufferObjectAttributesPtr attributes, unsigned int width, unsigned int height, GLenum target = GL_TEXTURE_2D);
        CameraPtr _camera;
};

}; // namespace

#endif // includeguard
