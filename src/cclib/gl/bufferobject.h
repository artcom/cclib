#ifndef __CCLIB_BUFFEROBJECT_INCLUDED__
#define __CCLIB_BUFFEROBJECT_INCLUDED__

namespace cclib {
    
// Usage Frequency:
#define BUFFERFREQ_STREAM  0x01
#define BUFFERFREQ_STATIC  0x02
#define BUFFERFREQ_DYNAMIC 0x03
    
// Usage Type:
#define BUFFERUSAGE_DRAW 0x01 
#define BUFFERUSAGE_READ 0x02
#define BUFFERUSAGE_COPY 0x03

class BufferObject {
    // Buffer Targets:	
    //    GL_ARRAY_BUFFER 
    //    GL_COPY_READ_BUFFER 
    //    GL_COPY_WRITE_BUFFER 
    //    GL_ELEMENT_ARRAY_BUFFER
    //    GL_PIXEL_PACK_BUFFER 
    //    GL_PIXEL_UNPACK_BUFFER
    //    GL_TEXTURE_BUFFER
    //    GL_TRANSFORM_FEEDBACK_BUFFER
    //    GL_UNIFORM_BUFFER

    private: 
        GLuint _myBufferID;
        int _mySize;
        bool _myIsMapped;  //= false;
	
        std::vector<char> _myData; // ByteBuffer
        GLenum _myCurrentTarget;

    public:
        CCBufferObject(int theSize=0) :
            _myBufferID(0), _mySize(0)    
        {
            _mySize = 0;
            glGenBuffers(1, &(_myBufferID));
            
            if (theSize>0) {
                bind(GL_ARRAY_BUFFER);
                bufferData(theSize, NULL, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
                updateData();
            } 
        }
	
	    void updateData() {
            bind(GL_ARRAY_BUFFER);
            
            // Initialize data store of buffer object
            _myData = mapBuffer();
            
            if(_myData == null) {
                switch(glGetError()) {
                case GL_NO_ERROR:
                    throw new Exception(" # NO ERROR REPORTED");
                    break;
                case GL_INVALID_ENUM:
                    throw new Exception(" # INVALID ENUMERATION REPORTED. check for errors in OPENGL calls with constants.");
                    break;
                case GL_INVALID_VALUE:
                    throw new Exception("# INVALID VALUE REPORTED. check for errors with passed values that are out of a defined range.");
                    break;
                case GL_INVALID_OPERATION:
                    throw new Exception("# INVALID OPERATION REPORTED. check for function calls that are invalid in the current graphics state.");
                    break;
                case GL_STACK_OVERFLOW:
                    throw new Exception("# STACK OVERFLOW REPORTED. check for errors in matrix operations");
                    break;
                case GL_STACK_UNDERFLOW:
                    throw new Exception("# STACK UNDERFLOW REPORTED. check for errors  in matrix operations");
                    break;
                case GL_OUT_OF_MEMORY:
                    throw new Exception("# OUT OF MEMORY. not enough memory to execute the commands");
                    break;
                case GL_TABLE_TOO_LARGE:
                    throw new Exception("# TABLE TOO LARGE.");
                    break;
                }
            }

            if(!_myData.empty()) {
                _myData.order(ByteOrder.nativeOrder());
            }
            
            unbind();
        }
	
        ByteBuffer data(){
            if(_myData == null){
                bind(GL_ARRAY_BUFFER);
                _myData = mapBuffer();
            }
            return _myData;
        }
	
        ByteBuffer mapBuffer() {
            _myIsMapped = true;
            return glMapBuffer(_myCurrentTarget, GL_WRITE_ONLY);
        }
	
        bool unmapBuffer() {
            _myIsMapped = false;
            return glUnmapBuffer(_myCurrentTarget);
        }
	
        bool isMapped() {
            return _myIsMapped;
        }
	
        GLuint id() {
            return _myBufferID;
        }
        
        void bind(GLenum theTarget){
            _myCurrentTarget = theTarget;
            glBindBuffer(theTarget, _myBufferID);
        }
        
        void unbind(){
            glBindBuffer(_myCurrentTarget, 0);
        }
    
    private: 
        GLunit glUsage(unsigned int theUsageFrequency, unsigned int theUsageType) {
            switch(theUsageFrequency) {
                case BUFFERFREQ_STREAM:
                    switch (theUsageType) {
                        case BUFFERUSAGE_DRAW:
                            return GL_STREAM_DRAW;
                        case BUFFERUSAGE_READ:
                            return GL_STREAM_READ;
                        case BUFFERUSAGE_COPY:
                            return GL_STREAM_COPY;
                    }
                case BUFFERFREQ_STATIC:
                    switch (theUsageType) {
                        case BUFFERUSAGE_DRAW:
                            return GL2ES2.GL_STATIC_DRAW;
                        case BUFFERUSAGE_READ:
                            return GL_STATIC_READ;
                        case BUFFERUSAGE_COPY:
                            return GL_STATIC_COPY;
                    }
                case BUFFERFREQ_DYNAMIC:
                    switch (theUsageType) {
                        case BUFFERUSAGE_DRAW:
                            return GL2ES2.GL_DYNAMIC_DRAW;
                        case BUFFERUSAGE_READ:
                            return GL_DYNAMIC_READ;
                        case BUFFERUSAGE_COPY:
                            return GL_DYNAMIC_COPY;
                    }
            }
            return 0;
        }

    public:
        void bufferData(int theSize, Buffer theData, CCUsageFrequency 
                theUsageFrequency, CCUsageTYPE theUsageType) {
            if(theData != null)theData.rewind();
            _mySize = theSize;
            glBufferData(_myCurrentTarget, theSize, 
                    theData, glUsage(theUsageFrequency, theUsageType));
        }
	
        void bufferData(int theSize, Buffer theData) {
            bufferData(theSize, theData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        }
        
        void bufferData() {
            bufferData(_mySize, _myData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        }

        void bufferSubData(CCBufferTarget theTarget, int theOffset, int theSize, Buffer theData) {
            if(theData != null)theData.rewind();
            _mySize = theSize;
            CCGraphics.currentGL().glBufferSubData(theTarget.glId, theOffset, theSize, theData);
        }
        
	void copyDataFromTexture(CCShaderTexture theShaderTexture, final int theID, final int theX, final int theY, final int theWidth, final int theHeight){
		GL2 gl = CCGraphics.currentGL();
    	int myNewBufferSize = theWidth * theHeight * theShaderTexture.numberOfChannels() * 4;
		if(myNewBufferSize != _mySize){
	    	bind(CCBufferTarget.ARRAY);	
	    	bufferData(
	    		myNewBufferSize, 
	    		null, 
	    		BUFFERFREQ_STREAM,
	    		BUFFERUSAGE_COPY
	    	);
			unbind();
			_mySize = myNewBufferSize;
		}
    	
    	theShaderTexture.bindBuffer();
    	// bind buffer object to pixel pack buffer
    	bind(CCBufferTarget.PIXEL_PACK);	
    	gl.glReadBuffer(GL_COLOR_ATTACHMENT0 + theID);
    	
    	// read from frame buffer to buffer object
    	gl.glReadPixels(theX, theY, theWidth,theHeight, theShaderTexture.format().glID, GL_FLOAT, 0);

    	unbind();
    	
    	theShaderTexture.unbindBuffer();
	}
	
	@Override
	protected void finalize() throws Throwable {
		CCGraphics.currentGL().glDeleteBuffers(1, _myBufferID, 0);
	}
}
}; // namespace

#endif
