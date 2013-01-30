#ifndef __CCLIB_BUFFEROBJECT_INCLUDED__
#define __CCLIB_BUFFEROBJECT_INCLUDED__

#include <cclib.h>

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
	
        std::vector<float> _myData; // ByteBuffer
        GLenum _myCurrentTarget;
        
        BufferObject(int theSize);

    public:
        BufferObjectPtr create(int theSize=0);
	    void updateData();
        std::vector<float> data(); // ByteBuffer
        std::vector<float> mapBuffer();
        bool unmapBuffer();
        bool isMapped();
        GLuint id();
        void bind(GLenum theTarget);
        void unbind();
        void bufferData(int theSize, std::vector<float> & theData, 
                int theUsageFrequency, int theUsageType);
        void bufferData(int theSize, std::vector<float> & theData);
        void bufferData();
        void bufferSubData(GLenum theTarget, int theOffset, int theSize, std::vector<float> & theData);
        void copyDataFromTexture(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight);
        
        virtual ~BufferObject();
    
    private: 
        GLuint glUsage(unsigned int theUsageFrequency, unsigned int theUsageType);
};

}; // namespace

#endif
