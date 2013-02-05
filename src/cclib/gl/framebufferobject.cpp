
#include <gl/framebufferobject.h>

using namespace cclib;

FrameBufferObject::FrameBufferObject(GLenum target, FrameBufferObjectAttributesPtr attributes,
            unsigned int width, unsigned int height) 
: Texture2D(attributes, width, height, attributes->numberOfColorBuffers, target),
 _renderFramebufferID(0), _depthTexture(), _bindIndex(0),
 _renderBufferIDs(), _attributes(attributes), _numberOfAttachments(attributes->numberOfColorBuffers)
{
    _useMultisampling = (attributes->numberOfSamples > 0); 
    _framebuffers = std::vector<GLuint>(2, 0);
    glGenFramebuffers(_framebuffers.size(), &(_framebuffers[0]));
    _drawBuffers = std::vector<GLuint>(_numberOfAttachments, 0);
    
    // if we don't need any variety of multisampling or it failed to initialize
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[0]);

    for(int i=0; i<_numberOfAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, _target, id(i), 0);
        _drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    if( !(_useMultisampling) || !initMultisampling()) { 
        _useMultisampling = false;
        init();
    }
}

void
FrameBufferObject::checkStatusException() {
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch( status ) {
        case GL_FRAMEBUFFER_COMPLETE:
            return;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw new Exception("Unsupported framebuffer format");
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw new Exception("Framebuffer incomplete: missing attachment");
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw new Exception("Framebuffer incomplete: duplicate attachment");
        // case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        //     throw new Exception("Framebuffer incomplete: attached images must have same dimensions");
        // case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        //     throw new Exception("Framebuffer incomplete: attached images must have same format");
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw new Exception("Framebuffer incomplete: missing draw buffer");
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw new Exception("Framebuffer incomplete: missing read buffer");
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
    if(_attributes->depthBuffer) {
        TextureAttributesPtr depthTextureAttributes = TextureAttributesPtr(new TextureAttributes());
        depthTextureAttributes->filter = GL_LINEAR;
        depthTextureAttributes->wrapS = GL_CLAMP_TO_EDGE;
        depthTextureAttributes->wrapT = GL_CLAMP_TO_EDGE;
        depthTextureAttributes->internalFormat = GL_DEPTH_COMPONENT24;
        depthTextureAttributes->format = GL_DEPTH_COMPONENT;
        depthTextureAttributes->type = GL_FLOAT;

        _depthTexture = Texture2D::createTexture2D(depthTextureAttributes, _width, _height, 1, _target);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _target, _depthTexture->id(), 0);
    }

    // checkStatusException();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
	
bool 
FrameBufferObject::initMultisampling() {
    _renderFramebufferID = 1;
    _maxAntialiasing = getMaxSamples();

    int samples = _attributes->numberOfSamples <= _maxAntialiasing ? _attributes->numberOfSamples : _maxAntialiasing;

    // create Render Buffer ids
    _renderBufferIDs = std::vector<GLuint>(_numberOfAttachments + 1, 0);
    glGenRenderbuffers(_renderBufferIDs.size(), &(_renderBufferIDs[0]));
    
    // create Multi sample depth buffer
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferIDs[0]);
    glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, samples, 
            GL_DEPTH_COMPONENT, _width, _height );

    // create Multi sample colorbuffers
    for(int i = 0; i < _numberOfAttachments;i++) {
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferIDs[1 + i]);
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, samples, 
                GL_RGBA, _width, _height);
    }

    // Attach them
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBufferIDs[0]);

    for(int i = 0; i < _numberOfAttachments;i++) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, _renderBufferIDs[1]);
    }

    // Final framebuffer color buffers
    textureFilter(GL_LINEAR);

    checkStatusException();
    return checkStatus();
}

bool 
FrameBufferObject::initMultisample() {
    _depthRenderBufferId = 0; 
    glGenRenderbuffers(1, &(_depthRenderBufferId));

    _colorRenderBufferId = std::vector<GLuint>(_attributes->numberOfColorBuffers, 0);
    glGenRenderbuffers(_attributes->numberOfColorBuffers, &(_colorRenderBufferId[0]));

    _resolveFramebufferId = 0; 
    glGenFramebuffers( 1, &(_resolveFramebufferId));

    // multisample, so we need to resolve from the FBO, bind the texture to the resolve FBO
    glBindFramebuffer(GL_FRAMEBUFFER, _resolveFramebufferId);

    for(int i=0; i<_attributes->numberOfColorBuffers; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                _target, id(i), 0);
    }

    // see if the resolve buffer is ok
    if(!checkStatus()) {
        return false;
    }

    if(_attributes->numberOfSamples > getMaxSamples() ) {
        _attributes->numberOfSamples = getMaxSamples();
    }

    // setup the primary framebuffer
    for(int i=0; i<_attributes->numberOfColorBuffers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[0] );
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBufferId[i]);


        // create a regular MSAA color buffer
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, _attributes->numberOfSamples, 
                _attributes->internalFormat, _width, _height );

        // attach the multisampled color buffer
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                GL_RENDERBUFFER, _colorRenderBufferId[i] );
    }

    if(_attributes->depthBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBufferId);
        // create the multisampled depth buffer (with or without coverage sampling)

        // create a regular (not coverage sampled) MSAA depth buffer
        glRenderbufferStorageMultisample(
                GL_RENDERBUFFER, _attributes->numberOfSamples, 
                _attributes->depthInternalFormat, _width, _height);

        // attach the depth buffer
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
                GL_RENDERBUFFER, _depthRenderBufferId);
    }

    // see if the primary framebuffer turned out ok
    return checkStatus();
}
	
FrameBufferObjectAttributesPtr 
FrameBufferObject::attributes() {
    return _attributes;
}
	
int 
FrameBufferObject::numberOfAttachments() {
    return _numberOfAttachments;
}
	
void 
FrameBufferObject::updateMipmaps() {
    if(!_generateMipmaps) {
        return;
    }

    for(int i=0; i<_attributes->numberOfColorBuffers; i++) {
        Texture2D::bind(i);
        glGenerateMipmap(_target);
    }
}
	
void 
FrameBufferObject::bind() {
    Texture2D::bind(_bindIndex);
}

void 
FrameBufferObject::bind(int theId) {
    Texture::bind(theId);
}

void 
FrameBufferObject::bindIndex(int bindIndex) {
    _bindIndex = bindIndex;
}

Texture2DPtr 
FrameBufferObject::depthTexture() {
    return _depthTexture;
}

void 
FrameBufferObject::bindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[0]);
}

void 
FrameBufferObject::unbindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void 
FrameBufferObject::bindFBO() {
    // Directing rendering to the texture...
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[_renderFramebufferID]);
    glDrawBuffers(_numberOfAttachments, &(_drawBuffers[0]));
}

void 
FrameBufferObject::bindFBO(int theTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[0]);
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + theTexture);
}

void 
FrameBufferObject::releaseFBO() {
    if(_useMultisampling) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffers[1]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffers[0]);
        glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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


