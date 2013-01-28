#include <gl/shadertexture.h>

using namespace cclib;

FrameBufferObjectAttributesPtr 
ShaderTexture::createAttributes( int theNumberOfBits, int theNumberOfChannels, int theNumberOfTextures) {
    FrameBufferObjectAttributesPtr result = FrameBufferObjectAttributesPtr(new FrameBufferObjectAttributes());

    result->depthBuffer = false;
    result->filter = GL_NEAREST;
    result->numberOfColorBuffers = theNumberOfTextures;
    result->type = GL_FLOAT;
    result->wrapS = GL_CLAMP;
    result->wrapT = GL_CLAMP;

    result->numberOfChannels = theNumberOfChannels;
    result->numberOfBits = theNumberOfBits; 

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

    result->internalFormat = internalFormat;
    result->format = format;

    return result;
}

void 
ShaderTexture::beginOrtho2D() {
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, _width, _height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, _width, 0, _height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}
        
void 
ShaderTexture::drawQuad() {
    switch (_target) {
        case GL_TEXTURE_2D:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(_width, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(_width, _height);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, _height);
            glEnd();
            break;

        default:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(_width, 0.0f);
            glVertex2f(_width, 0.0f);
            glTexCoord2f(_width, _height);
            glVertex2f(_width, _height);
            glTexCoord2f(0.0f, _height);
            glVertex2f(0.0f, _height);
            glEnd();
            break;
    }
}

void 
ShaderTexture::clear() {
    beginDraw();
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    endDraw();
}
	
int 
ShaderTexture::numberOfChannels() {
    return _numberOfChannels;
}

int 
ShaderTexture::numberOfBits() {
    return _numberOfBits;
}
	
void 
ShaderTexture::beginDraw() {
    bindFBO();
    beginOrtho2D();
}
	
void 
ShaderTexture::beginDraw(int texture) {
    bindFBO(texture);
    beginOrtho2D();
}

void 
ShaderTexture::endOrtho2D() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}
	
void 
ShaderTexture::endDraw() {
    endOrtho2D();
    releaseFBO();
}

void 
ShaderTexture::draw() {
    beginDraw();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    drawQuad();

    endDraw();
}
	
std::vector<float> 
ShaderTexture::getData(unsigned int x, unsigned int y, int width, int height, int texture) {
	if (width == -1) {
        width = _width;
    }
	if (height == -1) {
        height = _height;
    }
    
    std::vector<float> pixels(_width * _height * _numberOfChannels); 

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[0]);
	glReadBuffer(_drawBuffers[texture]);
	glReadPixels(x, y, width, height, _format, GL_FLOAT, &(pixels[0]));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	checkError();

	return pixels;
}

ShaderTexturePtr 
ShaderTexture::create(unsigned int theWidth, unsigned int theHeight, int theNumberOfBits, 
        int theNumberOfChannels, int theNumberOfTextures, GLenum theTarget) 
{
    FrameBufferObjectAttributesPtr attributes = createAttributes(theNumberOfBits, theNumberOfChannels, theNumberOfTextures);
    ShaderTexturePtr shaderTex = ShaderTexturePtr(new ShaderTexture(theWidth, theHeight, attributes, theTarget));

    return shaderTex;
}

ShaderTexture::ShaderTexture ( unsigned int theWidth, unsigned int theHeight, 
        FrameBufferObjectAttributesPtr theAttributes, GLenum theTarget) :
    FrameBufferObject(theTarget, theAttributes, theWidth, theHeight)

{
    _numberOfChannels = theAttributes->numberOfChannels;
    _numberOfBits = theAttributes->numberOfBits;

    _pbo = std::vector<PBOPtr>();
    _pbo.push_back(PBO::create(_numberOfChannels * theWidth * theHeight * (_numberOfBits == 16 ? 2 : 4)));
    _pbo.push_back(PBO::create(_numberOfChannels * theWidth * theHeight * (_numberOfBits == 16 ? 2 : 4)));
}


