#ifndef __CCLIB_SHADERTEXTURE_INCLUDED__
#define __CCLIB_SHADERTEXTURE_INCLUDED__

#include <cclib.h>
#include <gl/framebufferobject.h>

namespace cclib {

class PBO {

    public:

        static PBOPtr create(unsigned int dataSize) {
            return PBOPtr(new PBO(dataSize));
        };
        
        void beginUnpack() {
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id);
        }
	
        void endUnpack() {
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
	
        void beginPack() {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
        }
	
        void endPack() {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }
	
        // ByteBuffer mapBuffer(){
        //     return glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        // }
	
        // ByteBuffer mapReadBuffer() {
        //     glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
        //     return glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        // }
	
        // void unmapReadBuffer() {
        //     glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
        //     glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        // }


        ~PBO() {
            glDeleteBuffers(1, &_id);
        };

    private: 
        GLuint _id;

        PBO(unsigned int dataSize) {
            glGenBuffers(1, &_id);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
            glBufferData(GL_PIXEL_PACK_BUFFER, dataSize, NULL, GL_STREAM_READ);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        };
};

class ShaderTexture : public FrameBufferObject {

    public:

        static FrameBufferObjectAttributesPtr createAttributes( int theNumberOfBits, int theNumberOfChannels, int theNumberOfTextures); 
        static ShaderTexturePtr create(unsigned int theWidth, unsigned int theHeight, int theNumberOfBits=32, 
            int theNumberOfChannels=3, int theNumberOfTextures=1, GLenum theTarget=GL_TEXTURE_RECTANGLE); 

        void beginOrtho2D();
        void drawQuad(); 
        void clear();
        int numberOfChannels();
	    int numberOfBits();
        void beginDraw();
        void beginDraw(int texture);
        void endOrtho2D();
        void endDraw();
        void draw();
        std::vector<float> getData(unsigned int x=0, unsigned int y=0, int width=-1, int height=-1, int texture=0);
	
        // public void draw(CCAABoundingRectangle theRectangle) {
        //     beginDraw();
        //     GL2 gl = CCGraphics.currentGL();
        //     glBegin(GL_QUADS);
        //     glTexCoord2f(theRectangle.min().x, theRectangle.min().y);
        //     glVertex2f(theRectangle.min().x, theRectangle.min().y);
        //     glTexCoord2f(theRectangle.max().x, theRectangle.min().y);
        //     glVertex2f(theRectangle.max().x, theRectangle.min().y);
        //     glTexCoord2f(theRectangle.max().x, theRectangle.max().y);
        //     glVertex2f(theRectangle.max().x, theRectangle.max().y);
        //     glTexCoord2f(theRectangle.min().x, theRectangle.max().y);
        //     glVertex2f(theRectangle.min().x, theRectangle.max().y);
        //     glEnd();
        //     endDraw();
        // }

    private: 
        ShaderTexture ( unsigned int theWidth, unsigned int theHeight, 
                FrameBufferObjectAttributesPtr theAttributes, GLenum theTarget=GL_TEXTURE_RECTANGLE ); 
        std::vector<PBOPtr> _pbo;
        int _numberOfChannels;
        int _numberOfBits;
};

}; // namespace
	
	// private int i = 0;
	
	// public FloatBuffer getPBOData(final int theTexture) {
	// 	return getPBOData(theTexture, 0, 0, _myWidth, _myHeight);
	// }
	
	/**
	 * @param theTexture
	 * @param theX
	 * @param theY
	 * @param theWidth
	 * @param theHeight
	 * @return
	 */
// 	public FloatBuffer getPBOData(final int theTexture, final int theX, final int theY, final int theWidth, final int theHeight) {
// 		FloatBuffer myResult = FloatBuffer.allocate(theWidth * theHeight * _myNumberOfAttachments);
// 		
// 		GL2 gl = CCGraphics.currentGL();
// 		glBindFramebuffer(GL_FRAMEBUFFER, _myFrameBuffers[0]);
// 		glReadBuffer(_myDrawBuffers[theTexture]);
// 		
// 		_myPBO[i % 2].beginPack();
// 		glReadPixels(theX, theY, theWidth, theHeight,_myFormat.glID,GL_FLOAT,0);
// 		_myPBO[i % 2].endPack();
// 		
// 		myResult = _myPBO[(i + 1) % 2].mapReadBuffer().asFloatBuffer();
// 		_myPBO[(i + 1) % 2].unmapReadBuffer();
// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 		i++;
// 		return myResult;
// 	}
// 	
// 	/**
// 	 * Read data from a floatbuffer
// 	 * @param theData
// 	 */
// 	public void loadData(final FloatBuffer theData){
// 		theData.rewind();
// 		GL gl = CCGraphics.currentGL();
// 		glEnable(_myTarget.glID);
// 		glBindTexture(_myTarget.glID,_myTextureIDs[0]);
// 		glTexImage2D(_myTarget.glID,0,_myInternalFormat.glID,_myWidth,_myHeight,0,_myFormat.glID,GL_FLOAT,theData);
// 		glBindTexture(_myTarget.glID,0);
// 		glDisable(_myTarget.glID);
// 	}
// 
// }

#endif // includeguard
