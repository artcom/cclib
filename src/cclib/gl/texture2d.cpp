#include "texture2d.h"
#include <gl/pixelstoragemodes.h>
#include <gl/graphics.h>

using namespace cclib;

/**
 * Sets the color using values specified by the HSB model.
 * <ul>
 * <li>hue should be floating-point values between zero and one</li>
 * <li>saturation should be floating-point values between zero and one</li>
 * <li>brightness should be floating-point values between zero and one</li>
 * </ul>
 * The <code>saturation</code> and <code>brightness</code> components
 * should be floating-point values between zero and one
 * (numbers in the range 0.0-1.0).  The <code>hue</code> component
 * can be any floating-point number.  The floor of this number is
 * subtracted from it to create a fraction between 0 and 1.  This
 * fractional number is then multiplied by 360 to produce the hue
 * angle in the HSB color model.
 * <p>
 * The integer that is returned by <code>HSBtoRGB</code> encodes the
 * value of a color in bits 0-23 of an integer value that is the same
 * format used by the method {@link #getRGB() <code>getRGB</code>}.
 * This integer can be supplied as an argument to the
 * <code>Color</code> constructor that takes a single integer argument.
 * @param     theHue   the hue component of the color
 * @param     theSaturation   the saturation of the color
 * @param     theBrightness   the brightness of the color
 * @return    the RGB value of the color with the indicated hue,
 *                            saturation, and brightness.
 */
void cclib::Color::setHSB(float theHue, float theSaturation, float theBrightness, float theAlpha)
{
    _alpha = theAlpha;
    
    if (theSaturation == 0) {
        _red = _green = _blue = theBrightness;
    } else {
        float which = (theHue - (int) theHue) * 6.0f;
        float f = which - (int) which;
        float p = theBrightness * (1.0f - theSaturation);
        float q = theBrightness * (1.0f - theSaturation * f);
        float t = theBrightness * (1.0f - (theSaturation * (1.0f - f)));
        
        switch ((int) which)
        {
			case 0:
				_red = theBrightness;
				_green = t;
				_blue = p;
				break;
			case 1:
				_red = q;
				_green = theBrightness;
				_blue = p;
				break;
			case 2:
				_red = p;
				_green = theBrightness;
				_blue = t;
				break;
			case 3:
				_red = p;
				_green = q;
				_blue = theBrightness;
				break;
			case 4:
				_red = t;
				_green = p;
				_blue = theBrightness;
				break;
			case 5:
				_red = theBrightness;
				_green = p;
				_blue = q;
				break;
        }
    }

}

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


