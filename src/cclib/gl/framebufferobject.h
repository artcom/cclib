#ifndef __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__
#define __CCLIB_FRAMEBUFFEROBJECT_INCLUDED__

#include <cclib.h>
#include <gl/texture.h>
#include <gl/texture2d.h>
#include <gl/pixelstoragemodes.h>


namespace cclib {

class FrameBufferObjectAttributes {
    public:
        
        GLenum depthInternalFormat;
        std::vector<TextureAttributesPtr> textureAttributes;

        int numberOfSamples;
        int coverageSamples;
        bool depthBuffer;
        bool stencilBuffer;
        int numberOfColorBuffers;
        int numberOfBits;
        int numberOfChannels;

        FrameBufferObjectAttributes(TextureAttributesPtr theAttributes, int numberOfAttachments=1) :
            depthInternalFormat(GL_DEPTH_COMPONENT24),
            numberOfSamples(0),
            coverageSamples(0),
            depthBuffer(true),
            stencilBuffer(false),
            numberOfColorBuffers(numberOfAttachments),
            numberOfBits(32),
            numberOfChannels(4)
        {
            for (int i=0; i<numberOfAttachments; i++) {
                textureAttributes.push_back(theAttributes);
            }
            
//            printf("%s\n\tnumberOfSamples %d\n\tnumberOfBits %d\n\tnumberOfChannels %d\n\tnumberOfAttachments %d\n\tnumberOfSamples %d\n",__PRETTY_FUNCTION__,
//                   theAttributes->numberOfSamples,theAttributes->numberOfBits,theAttributes->numberOfChannels,
//                   numberOfAttachments,
//                   numberOfSamples);
        };
    
        static FrameBufferObjectAttributesPtr create(TextureAttributesPtr theAttributes, int numberOfAttachments=1)
        {
            return cclib::FrameBufferObjectAttributesPtr(new cclib::FrameBufferObjectAttributes(theAttributes, numberOfAttachments));
        };
    
        virtual ~FrameBufferObjectAttributes() {
        };
};

class FrameBufferObject {
    
    public:
        FrameBufferObject() {};
        FrameBufferObject(GLenum target, FrameBufferObjectAttributesPtr attributes,
                          unsigned int width, unsigned int height);
    
        virtual ~FrameBufferObject();
    
        static FrameBufferObjectPtr create(GLenum target, FrameBufferObjectAttributesPtr attributes,
                                                  unsigned int width, unsigned int height) {
            return FrameBufferObjectPtr(new FrameBufferObject(target, attributes, width, height));
        };
        
        static int getMaxSamples();

        void checkStatusException();
        bool checkStatus();
        void init();
        bool initMultisampling();
        bool initMultisample();
        
        FrameBufferObjectAttributesPtr attributes();
    
        Texture2DPtr attachment(int theId);
        int numberOfAttachments();
        void updateMipmaps();
        
        Texture2DPtr depthTexture();
        void bindBuffer();
        void unbindBuffer();
        void bindFBO();
        void bindFBO(int theTexture);
        void releaseFBO();
        unsigned int width();
        unsigned int height();

        virtual void beginDraw() {};
        virtual void endDraw() {};
    
    public:
        int _myWidth;
        int _myHeight;
    
    private:
        GLuint _myRenderFramebufferID;
    
        std::vector<GLuint> _myRenderBufferIDs;
        bool _myUseMultisampling;
    
        int _myNumberOfAttachments;
    //
        Texture2DPtr _myDepthTexture;
 /*       GLuint _myBindIndex;
*/
        unsigned int _myMaxAntialiasing;
        FrameBufferObjectAttributesPtr _myAttributes;

        GLuint _myDepthRenderBufferId;
        std::vector<GLuint> _myColorRenderBufferId;
        GLuint _myResolveFramebufferId;

    protected:

        std::vector<Texture2DPtr> _myAttachments;

        std::vector<GLuint> _myFramebuffers;
        std::vector<GLuint> _myDrawBuffers;
        GLenum _myTarget;
};
};
#endif // includeguard
	
