#ifndef __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__
#define __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__

#include <cclib.h>
#include <gl/texture.h>
#include <gl/texture2d.h>
#include <gl/pixelstoragemodes.h>


namespace cclib {

class FrameBufferObjectAttributes : public TextureAttributes {
    public:
        
        GLenum depthInternalFormat;
        int numberOfSamples;
        int coverageSamples;
        bool depthBuffer;
        bool stencilBuffer;
        int numberOfColorBuffers;
        
        int numberOfBits;
        int numberOfChannels;

        FrameBufferObjectAttributes(int numberOfAttachments=1) : 
            TextureAttributes(),
            depthInternalFormat(GL_DEPTH_COMPONENT24),
            numberOfSamples(0),
            coverageSamples(0),
            numberOfColorBuffers(numberOfAttachments),
            depthBuffer(true),
            stencilBuffer(false),
            numberOfChannels(1),
            numberOfBits(8)
        {
            internalFormat = GL_RGBA8;
        }; 

        TextureAttributesPtr getTex2DAttributes() {
            return TextureAttributesPtr(static_cast<TextureAttributes*>(this));
        };
};

class FrameBufferObject : public Texture2D {
    public:
        virtual ~FrameBufferObject() {}; 
        
        static int getMaxSamples();

        void checkStatusException();
        bool checkStatus();
        void init();
        bool initMultisampling();
        bool initMultisample();
        
        FrameBufferObjectAttributesPtr attributes();
        int numberOfAttachments();
        void updateMipmaps();
        void bind();
        void bind(int theId);
        void bindIndex(int theBindIndex);
        
        Texture2DPtr depthTexture();
        void bindBuffer();
        void unbindBuffer();
        void bindFBO();
        void bindFBO(int theTexture);
        void releaseFBO();
	
        virtual void beginDraw() = 0;
        virtual void endDraw() = 0;
    
    private:
        
        GLuint _renderFramebufferID;
        Texture2DPtr _depthTexture;
        GLuint _bindIndex;

        std::vector<GLuint> _renderBufferIDs;
        bool _useMultisampling;

        FrameBufferObjectAttributesPtr _attributes;
	    int _maxAntialiasing;
	
        GLuint _depthRenderBufferId;
        std::vector<GLuint> _colorRenderBufferId;
        GLuint _resolveFramebufferId;

    protected:
        FrameBufferObject(GLenum target, FrameBufferObjectAttributesPtr attributes,
                    unsigned int width, unsigned int height); 
        
        std::vector<GLuint> _framebuffers;
        std::vector<GLuint> _drawBuffers;
        int _numberOfAttachments;
};
};
#endif // includeguard
	
