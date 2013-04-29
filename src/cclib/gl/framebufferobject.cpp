
#include <gl/framebufferobject.h>
#include <gl/graphics.h>

using namespace cclib;

FrameBufferObject::FrameBufferObject(GLenum theTarget, FrameBufferObjectAttributesPtr theAttributes,
            unsigned int theWidth, unsigned int theHeight)
: _myTarget(theTarget), _myRenderFramebufferID(0),
 _myWidth(theWidth), _myHeight(theHeight),
 _myRenderBufferIDs(), _myAttributes(theAttributes),
 _myNumberOfAttachments(theAttributes->numberOfColorBuffers)
{
    for (unsigned int i=0; i<_myNumberOfAttachments; i++) {
        _myAttachments.push_back(Texture2D::create(theAttributes->textureAttributes[i], _myWidth, _myHeight, 1, theTarget));
    }
                                 
    _myUseMultisampling = (_myAttributes->numberOfSamples > 0);
    _myFramebuffers = std::vector<GLuint>(2, 0);
    glGenFramebuffers(_myFramebuffers.size(), &(_myFramebuffers[0]));
    _myDrawBuffers = std::vector<GLuint>(_myNumberOfAttachments, 0);
    Graphics::checkError();
    
    glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[0]);
    Graphics::checkError();
    
    for(int i=0; i<_myNumberOfAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, _myTarget, _myAttachments[i]->id(), 0);
        _myDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        Graphics::checkError();
    }

//    printf("%s\n\ttheTarget %#x\n\tmyFramebuffers[0] %d\n\tnumberOfSamples %d\n\tnumberOfBits %d\n\tnumberOfChannels %d\n\t_myUseMultisampling %d\n",__PRETTY_FUNCTION__,
//           (int)theTarget,
//           _myFramebuffers[0],
//           _myAttributes->numberOfSamples,_myAttributes->numberOfBits,_myAttributes->numberOfChannels,
//           (int)_myUseMultisampling);
    
    if( !(_myUseMultisampling) || !initMultisampling()) {
        _myUseMultisampling = false;
        init();
    }
    
    Graphics::checkError();
}

unsigned int
FrameBufferObject::width() {
    return _myWidth;
}

unsigned int
FrameBufferObject::height() {
    return _myHeight;
}

Texture2DPtr
FrameBufferObject::attachment(int theId) {
    if (_myAttachments.size() <= theId) {
        std::stringstream ss;
        ss << "Requesting attachment that isn't there: #" << theId << " (max: " << (_myAttachments.size() - 1) << ")";
        throw new Exception(ss.str());
    }
    return _myAttachments[theId];
}

void
FrameBufferObject::checkStatusException() {
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch( status ) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw new Exception("Unsupported framebuffer format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw new Exception("Framebuffer incomplete: missing attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw new Exception("Framebuffer incomplete: duplicate attachment");
        // case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        //     throw new Exception("Framebuffer incomplete: attached images must have same dimensions");
        // case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        //     throw new Exception("Framebuffer incomplete: attached images must have same format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw new Exception("Framebuffer incomplete: missing draw buffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw new Exception("Framebuffer incomplete: missing read buffer");
            break;
        default:
            throw new Exception("Framebuffer invalid: unknown reason");
    }
}
	
bool 
FrameBufferObject::checkStatus() {
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    return status == GL_FRAMEBUFFER_COMPLETE;
}
	
void 
FrameBufferObject::init() {
    // allocate and attach depth texture
    if(_myAttributes->depthBuffer) {
//        TextureAttributesPtr depthTextureAttributes = TextureAttributes::create(24, 1); // not supported ?
        TextureAttributesPtr depthTextureAttributes = TextureAttributes::create(32, 4);
        depthTextureAttributes->filter = GL_LINEAR;
        depthTextureAttributes->wrapS = GL_CLAMP_TO_EDGE;
        depthTextureAttributes->wrapT = GL_CLAMP_TO_EDGE;
        depthTextureAttributes->internalFormat = GL_DEPTH_COMPONENT24;
        depthTextureAttributes->format = GL_DEPTH_COMPONENT;
        depthTextureAttributes->type = GL_FLOAT;

        _myDepthTexture = Texture2D::create(depthTextureAttributes, _myWidth, _myHeight, 1, _myTarget);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _myTarget, _myDepthTexture->id(), 0);
        Graphics::checkError();
    }

    // checkStatusException();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Graphics::checkError();
}
	
bool 
FrameBufferObject::initMultisampling() {
    _myRenderFramebufferID = 1;
    _myMaxAntialiasing = getMaxSamples();

    int mySamples = _myAttributes->numberOfSamples <= _myMaxAntialiasing ? _myAttributes->numberOfSamples : _myMaxAntialiasing;

    // create Render Buffer ids
    _myRenderBufferIDs = std::vector<GLuint>(_myNumberOfAttachments + 1, 0);
    glGenRenderbuffers(_myRenderBufferIDs.size(), &(_myRenderBufferIDs[0]));
    
    // create Multi sample depth buffer
    glBindRenderbuffer(GL_RENDERBUFFER, _myRenderBufferIDs[0]);
    glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, mySamples,
            GL_DEPTH_COMPONENT, _myWidth, _myHeight );

    // create Multi sample colorbuffers
    for(int i = 0; i < _myNumberOfAttachments;i++) {
        glBindRenderbuffer(GL_RENDERBUFFER, _myRenderBufferIDs[1 + i]);
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, mySamples,
                GL_RGBA, _myWidth, _myHeight);
    }

    // Attach them
    glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _myRenderBufferIDs[0]);

    for(int i = 0; i < _myNumberOfAttachments; i++) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, _myRenderBufferIDs[1]);
        _myAttachments[i]->textureFilter(GL_LINEAR);
    }

    checkStatusException();
    return checkStatus();
}

bool 
FrameBufferObject::initMultisample() {
    _myDepthRenderBufferId = 0;
    glGenRenderbuffers(1, &(_myDepthRenderBufferId));

    _myColorRenderBufferId = std::vector<GLuint>(_myAttributes->numberOfColorBuffers, 0);
    glGenRenderbuffers(_myAttributes->numberOfColorBuffers, &(_myColorRenderBufferId[0]));

    _myResolveFramebufferId = 0;
    glGenFramebuffers( 1, &(_myResolveFramebufferId));

    // multisample, so we need to resolve from the FBO, bind the texture to the resolve FBO
    glBindFramebuffer(GL_FRAMEBUFFER, _myResolveFramebufferId);

    for(int i=0; i<_myAttributes->numberOfColorBuffers; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                _myTarget, _myAttachments[i]->id(), 0);
    }

    // see if the resolve buffer is ok
    if(!checkStatus()) {
        return false;
    }

    if(_myAttributes->numberOfSamples > getMaxSamples() ) {
        _myAttributes->numberOfSamples = getMaxSamples();
    }

    // setup the primary framebuffer
    for(int i=0; i<_myAttributes->numberOfColorBuffers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[0] );
        glBindRenderbuffer(GL_RENDERBUFFER, _myColorRenderBufferId[i]);


        // create a regular MSAA color buffer
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, _myAttributes->numberOfSamples,
                _myAttachments[i]->internalFormat(), _myWidth, _myHeight );

        // attach the multisampled color buffer
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                GL_RENDERBUFFER, _myColorRenderBufferId[i] );
    }

    if(_myAttributes->depthBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, _myDepthRenderBufferId);
        // create the multisampled depth buffer (with or without coverage sampling)

        // create a regular (not coverage sampled) MSAA depth buffer
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, _myAttributes->numberOfSamples,
                _myAttributes->depthInternalFormat, _myWidth, _myHeight);

        // attach the depth buffer
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
                GL_RENDERBUFFER, _myDepthRenderBufferId);
    }

    // see if the primary framebuffer turned out ok
    return checkStatus();
}
	
FrameBufferObjectAttributesPtr 
FrameBufferObject::attributes() {
    return _myAttributes;
}
	
int 
FrameBufferObject::numberOfAttachments() {
    return _myNumberOfAttachments;
}
	
void 
FrameBufferObject::updateMipmaps() {
    for(int i=0; i<_myAttachments.size(); i++) {
        if (!_myAttachments[i]->generateMipmaps()) {
            continue;
        }
        _myAttachments[i]->bind();
        glGenerateMipmap(_myTarget);
    }
}

Texture2DPtr 
FrameBufferObject::depthTexture() {
    return _myDepthTexture;
}

void 
FrameBufferObject::bindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[0]);
}

void 
FrameBufferObject::unbindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void 
FrameBufferObject::bindFBO() {
    // Directing rendering to the texture...
    glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[_myRenderFramebufferID]);
    glDrawBuffers(_myNumberOfAttachments, &(_myDrawBuffers[0]));
    Graphics::checkError();
}

void 
FrameBufferObject::bindFBO(int theTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[0]);

    Graphics::checkError();

    glDrawBuffer(GL_COLOR_ATTACHMENT0 + theTexture);
    Graphics::checkError();
}

void 
FrameBufferObject::releaseFBO() {
    if(_myUseMultisampling) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _myFramebuffers[1]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _myFramebuffers[0]);
        glBlitFramebuffer(0, 0, _myWidth, _myHeight, 0, 0, _myWidth, _myHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    updateMipmaps();
}

int 
FrameBufferObject::getMaxSamples() {
    if (!glewIsSupported("GL_EXT_framebuffer_multisample") || !glewIsSupported("GL_EXT_framebuffer_blit")) {
        return 0; 
    }

    int result; 
    glGetIntegerv(GL_MAX_SAMPLES, &result);
    
    return result;
};


