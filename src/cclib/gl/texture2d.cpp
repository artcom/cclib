#include "texture2d.h"
#include <gl/pixelstoragemodes.h>
#include <gl/graphics.h>

using namespace cclib;

Texture2DPtr 
Texture2D::create(TextureAttributesPtr attributes, int width, int height, 
        unsigned int numberOfTextures, GLenum target)
{
    Texture2DPtr texture = Texture2DPtr(new Texture2D(attributes, width, height, numberOfTextures, target));
    return texture;
}

Texture2DPtr
Texture2D::create(std::vector<unsigned char> theData, GLenum theTarget,
                  unsigned int theWidth, unsigned int theHeight)
{
    TextureAttributesPtr t = TextureAttributes::create(32, 4);
    return Texture2D::create(t, theWidth, theHeight, 1, GL_TEXTURE_RECTANGLE);
}

Texture2D::Texture2D(TextureAttributesPtr attributes,
             int width, int height, unsigned int numberOfTextures, GLenum target) 
    : Texture(target, attributes, numberOfTextures) 
{ 
    if (width > 0 && height > 0) {
        allocateData(width, height);
    }
}


void 
Texture2D::allocateData(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;

    for(unsigned int i = 0; i < _textureIDs.size(); i++) 
    {
        bind(i);
        _storageModes->unpackStorage();
        Graphics::checkError();
        
        glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, _format, _pixelType, NULL);
        
        Graphics::checkError();
        
        _storageModes->defaultUnpackStorage();
        Graphics::checkError();
        
    }
}

void
Texture2D::dataImplementation(const std::vector<unsigned char> & theData) {
    glTexImage2D( _target, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(theData[0]) );
}

// 	public ByteBuffer buffer() {
// 		int mySize = _myPixelType.bytesPerChannel * _myFormat.numberOfChannels * _myWidth * _myHeight;
// 		if(_myBufferObject == null) {
// 			_myBufferObject = new CCBufferObject();
// 			_myBufferObject.bind(CCBufferTarget.PIXEL_UNPACK);
// 	        _myBufferObject.bufferData(mySize, null, CCUsageFrequency.STREAM, CCUsageTYPE.DRAW);
// 		}
// 		_myBufferObject.bind(CCBufferTarget.PIXEL_UNPACK);
// 		return _myBufferObject.mapBuffer();
// 	}
	
	/**
	 * Sets the pixel at the given index to the given color.
	 * @param theX position of the pixel from the left side
	 * @param theY position of the pixel from the top side
	 * @param theColor the new color of the pixel
	 */
	// public void setPixel(final int theX, final int theY, final CCColor theColor) {
	// 	GL gl = CCGraphics.currentGL();
	// 	
	// 	FloatBuffer myBuffer = FloatBuffer.allocate(4);
	// 	myBuffer.put(theColor.red());
	// 	myBuffer.put(theColor.green());
	// 	myBuffer.put(theColor.blue());
	// 	myBuffer.put(theColor.alpha());
	// 	myBuffer.rewind();
	// 	
	// 	gl.glTexSubImage2D(
	// 		_myTarget.glID, 0, 
	// 		theX, theY, 1, 1,
	// 		CCPixelFormat.RGBA.glID, CCPixelType.FLOAT.glID, myBuffer
	// 	);
	// }
	
	// private CCPixelMap _myPixelMap;
	
	/**
	 * Returns the color for the pixel at the given index
	 * @param theX position of the pixel from the left side
	 * @param theY position of the pixel from the top
	 * @return the color of the pixel
	 */
	// public CCColor getPixel(final int theX, final int theY) {
	// 	if(_myPixelMap == null) {
	// 		FloatBuffer myPixelData = FloatBuffer.allocate(4 * _myWidth * _myHeight);
	// 		GL2 gl = CCGraphics.currentGL();
	// 		gl.glGetTexImage(_myTarget.glID, 0, CCPixelFormat.RGBA.glID, CCPixelType.FLOAT.glID, myPixelData);
	// 		myPixelData.rewind();
	// 		
	// 		_myPixelMap = new CCPixelMap(myPixelData, _myWidth, _myHeight, _myMustFlipVertically);
	// 	}
	// 	
	// 	return _myPixelMap.getPixel(theX, theY);
	// }
	
	/**
	 * Returns all pixels of the texture as pixelmap
	 * @return
	 */
	// public CCPixelMap getPixels() {
	// 	FloatBuffer myBuffer = FloatBuffer.allocate(4 * _myWidth * _myHeight);
	// 	glGetTexImage(_myTarget.glID, 0, CCPixelFormat.RGBA.glID, CCPixelType.FLOAT.glID, myBuffer);
	// 	myBuffer.rewind();
	// 	
	// 	return new CCPixelMap(myBuffer, _myWidth, _myHeight, _myMustFlipVertically);
	// }


