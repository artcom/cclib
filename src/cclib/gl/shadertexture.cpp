#include <gl/shadertexture.h>

#include <gl/graphics.h>

using namespace cclib;

FrameBufferObjectAttributesPtr 
ShaderBuffer::createAttributes( int theNumberOfBits, int theNumberOfChannels, int theNumberOfTextures) {
    TextureAttributesPtr myTextureAttributes = TextureAttributesPtr(new TextureAttributes());
    myTextureAttributes->type = GL_FLOAT;
    myTextureAttributes->wrapS = GL_CLAMP;
    myTextureAttributes->wrapT = GL_CLAMP;
    myTextureAttributes->filter = GL_NEAREST;
    
    bool is16Bit;

    switch(theNumberOfBits) {
        case 16:
            is16Bit = true;
            break;
        case 32:
            is16Bit = false;
            break;
        default:
            throw new cclib::Exception("The given number of bits is not supported. You can only create shader textures with 16 or 32 bit resolution.");
    }

    std::string vendorString = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    unsigned nvVendorIndex = vendorString.find(std::string("NVIDIA"));
    bool isNvidia = (nvVendorIndex != std::string::npos); 

    GLenum format;
    GLenum internalFormat;

    switch(theNumberOfChannels) {
        case 1:
            if(isNvidia) {
                internalFormat = is16Bit ? GL_FLOAT_R16_NV : GL_FLOAT_R32_NV;
            } else {
                internalFormat = is16Bit ? GL_LUMINANCE_FLOAT16_ATI : GL_LUMINANCE_FLOAT32_ATI;
            }
            format = GL_LUMINANCE;
            break;
        case 2:
            if(isNvidia) {
                internalFormat = is16Bit ? GL_FLOAT_RG16_NV : GL_FLOAT_RG32_NV;
            } else {
                internalFormat = is16Bit ? GL_LUMINANCE_ALPHA_FLOAT16_ATI : GL_LUMINANCE_ALPHA_FLOAT32_ATI;
            }
            format = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            internalFormat = is16Bit ? GL_RGB16F : GL_RGB32F;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = is16Bit ? GL_RGBA16F : GL_RGBA32F;
            format = GL_RGBA;
            break;
        default:
            throw new cclib::Exception("The given number of channels is not supported. You can only create shader textures with 1,2,3 or 4 channels.");

    }

    myTextureAttributes->internalFormat = internalFormat;
    myTextureAttributes->format = format;

    FrameBufferObjectAttributesPtr result = FrameBufferObjectAttributesPtr(new FrameBufferObjectAttributes(myTextureAttributes, theNumberOfTextures));
    result->numberOfBits = theNumberOfBits;
    result->numberOfChannels = theNumberOfChannels;

    result->depthBuffer = false;

    Graphics::checkError();
    return result;
}

void 
ShaderBuffer::beginOrtho2D() {
    Graphics::checkError();
    glPushAttrib(GL_VIEWPORT_BIT);
    Graphics::checkError();
    glViewport(0, 0, _myWidth, _myHeight);
    Graphics::checkError();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, _myWidth, 0, _myHeight, -1, 1);
    Graphics::checkError();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Graphics::checkError();
}
        
void 
ShaderBuffer::drawQuad() {
    switch (_myTarget) {
        case GL_TEXTURE_2D:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(_myWidth, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(_myWidth, _myHeight);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, _myHeight);
            glEnd();
            break;

        default:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(_myWidth, 0.0f);
            glVertex2f(_myWidth, 0.0f);
            glTexCoord2f(_myWidth, _myHeight);
            glVertex2f(_myWidth, _myHeight);
            glTexCoord2f(0.0f, _myHeight);
            glVertex2f(0.0f, _myHeight);
            glEnd();
            break;
    }
    
    Graphics::checkError();
}

void 
ShaderBuffer::clear() {
    Graphics::checkError();
    beginDraw();
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    endDraw();
    Graphics::checkError();
}

int 
ShaderBuffer::numberOfChannels() {
    return _numberOfChannels;
}

int 
ShaderBuffer::numberOfBits() {
    return _numberOfBits;
}
	
void 
ShaderBuffer::beginDraw() {
    bindFBO();
    beginOrtho2D();
}
	
void 
ShaderBuffer::beginDraw(int texture) {
    bindFBO(texture);
    beginOrtho2D();
}

void 
ShaderBuffer::endOrtho2D() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
    Graphics::checkError();
}

void 
ShaderBuffer::endDraw() {
    endOrtho2D();
    releaseFBO();
}

void 
ShaderBuffer::draw() {
    beginDraw();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    drawQuad();

    endDraw();
    Graphics::checkError();

}

std::vector<float> 
ShaderBuffer::getData(unsigned int theAttachment, unsigned int x, unsigned int y, int width, int height, int texture) {
	if (width == -1) {
        width = _myWidth;
    }
	if (height == -1) {
        height = _myHeight;
    }
    
    std::vector<float> pixels(_myWidth * _myHeight * _numberOfChannels); 
    
	glBindFramebuffer(GL_FRAMEBUFFER, _myFramebuffers[0]);
	glReadBuffer(_myDrawBuffers[texture]);
	glReadPixels(x, y, width, height, _myAttachments[theAttachment]->format(), GL_FLOAT, &(pixels[0]));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
    Graphics::checkError();

	return pixels;
}

ShaderBufferPtr 
ShaderBuffer::create(unsigned int theWidth, unsigned int theHeight, int theNumberOfBits, 
        int theNumberOfChannels, int theNumberOfTextures, GLenum theTarget) 
{
    FrameBufferObjectAttributesPtr attributes = createAttributes(theNumberOfBits, theNumberOfChannels, theNumberOfTextures);
    ShaderBufferPtr shaderTex = ShaderBufferPtr(new ShaderBuffer(theWidth, theHeight, attributes, theTarget));

    return shaderTex;
}

ShaderBuffer::ShaderBuffer ( unsigned int theWidth, unsigned int theHeight, 
        FrameBufferObjectAttributesPtr theAttributes, GLenum theTarget) :
    FrameBufferObject(theTarget, theAttributes, theWidth, theHeight)

{
    _numberOfChannels = theAttributes->numberOfChannels;
    _numberOfBits = theAttributes->numberOfBits;    

    _pbo = std::vector<PBOPtr>();
    _pbo.push_back(PBO::create(_numberOfChannels * theWidth * theHeight * (_numberOfBits == 16 ? 2 : 4)));
    _pbo.push_back(PBO::create(_numberOfChannels * theWidth * theHeight * (_numberOfBits == 16 ? 2 : 4)));
}


