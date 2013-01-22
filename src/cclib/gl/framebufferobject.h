#ifndef __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__
#define __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <gl/texture2d.h>
#include <Exception.h>

namespace cclib {

class FrameBufferObjectAttributes : public TextureAttributes {
    public:
        typedef std::tr1::shared_ptr<FrameBufferObjectAttributes> Ptr; 
        
        GLenum depthInternalFormat;
        int numberOfSamples;
        int coverageSamples;
        bool depthBuffer;
        bool stencilBuffer;
        int numberOfColorBuffers;
        
        FrameBufferObjectAttributes(int numberOfAttachments=1) : 
            TextureAttributes(),
            depthInternalFormat(GL_DEPTH_COMPONENT24),
            numberOfSamples(0),
            coverageSamples(0),
            numberOfColorBuffers(numberOfAttachments),
            depthBuffer(true),
            stencilBuffer(false)
    {
        internalFormat = GL_RGBA8;
    }; 

        TextureAttributes::Ptr getTex2DAttributes() {
            return TextureAttributes::Ptr(static_cast<TextureAttributes*>(this));
        };
};

class FrameBufferObject : public Texture2D {
    public:
        typedef std::tr1::shared_ptr<FrameBufferObject> Ptr; 
        virtual ~FrameBufferObject() {}; 
        
        static int getMaxSamples();

        void checkStatusException();
        bool checkStatus();
        void init();
        bool initMultisampling();
        bool initMultisample();
        
        FrameBufferObjectAttributes::Ptr attributes();
        int numberOfAttachments();
        void updateMipmaps();
        void bind();
        void bindIndex(int theBindIndex);
        
        Texture2D::Ptr depthTexture();
        void bindBuffer();
        void unbindBuffer();
        void bindFBO();
        void bindFBO(int theTexture);
        void releaseFBO();
	
        virtual void beginDraw() = 0;
        virtual void endDraw() = 0;
    
    private:
        
        GLuint _renderFramebufferID;
        Texture2D::Ptr _depthTexture;
        GLuint _bindIndex;

        std::vector<GLuint> _renderBufferIDs;
        bool _useMultisampling;

        FrameBufferObjectAttributes::Ptr _attributes;
	    int _maxAntialiasing;
	
        GLuint _depthRenderBufferId;
        std::vector<GLuint> _colorRenderBufferId;
        GLuint _resolveFramebufferId;

    protected:
        FrameBufferObject(GLenum target, FrameBufferObjectAttributes::Ptr attributes,
                    unsigned int width, unsigned int height); 
        
        std::vector<GLuint> _framebuffers;
        std::vector<GLuint> _drawBuffers;
        int _numberOfAttachments;
};
};
#endif // includeguard
	
