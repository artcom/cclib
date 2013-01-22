#include "texture2d.h"

using namespace cclib;

Texture2D::Ptr 
Texture2D::createTexture2D(TextureAttributes::Ptr attributes, int width, int height, 
        unsigned int numberOfTextures, GLenum target)
{
    Ptr texture = Ptr(new Texture2D(attributes, width, height, numberOfTextures, target));
    if (width > 0 && height > 0) {
        texture->allocateData(width, height);
    }

    return texture;
}

Texture2D::Texture2D(TextureAttributes::Ptr attributes,
             int width, int height, unsigned int numberOfTextures, GLenum target) 
    : Texture(target, attributes, numberOfTextures) 
{ 
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
        glTexImage2D(
                _target, 0, 
                _internalFormat, 
                _width, _height, 0, 
                _format, _pixelType, 
                NULL); 
        
        _storageModes->defaultUnpackStorage();
    }
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

void 
Texture2D::checkError() {
    GLenum error = glGetError();

    std::string errorStr;
    switch(error) {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            errorStr = " # INVALID ENUMERATION REPORTED. check for errors in OPENGL calls with constants.";
        case GL_INVALID_VALUE:
            errorStr = "# INVALID VALUE REPORTED. check for errors with passed values that are out of a defined range.";
        case GL_INVALID_OPERATION:
            errorStr = "# INVALID OPERATION REPORTED. check for function calls that are invalid in the current graphics state.";
        case GL_STACK_OVERFLOW:
            errorStr = "# STACK OVERFLOW REPORTED. check for errors in matrix operations";
        case GL_STACK_UNDERFLOW:
            errorStr = "# STACK UNDERFLOW REPORTED. check for errors  in matrix operations";
        case GL_OUT_OF_MEMORY:
            errorStr = "# OUT OF MEMORY. not enough memory to execute the commands";
        case GL_TABLE_TOO_LARGE:
            errorStr = "# TABLE TOO LARGE.";
    }

    throw cclib::Exception("Texture2D: "+ errorStr);
}
