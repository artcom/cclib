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

class Buffer {
public:
    Buffer(unsigned int theSize) :
        _myData(NULL), _myEmpty(true), _mySize(theSize), _myCurrentIndex(0)
    {
        _myData = new float[_mySize]();
    };
    
    virtual ~Buffer() {};
    
    void setData(float * theData) {
        if (_myData != NULL) {
            delete _myData;
        }
        
        if (theData) {
            _myData = theData;
            _myEmpty = false;
        }
    };

    unsigned int size() {
        return _mySize;
    };
    
    float * data() {
        if (empty()) {
            // XXX Exception
            return NULL;
        }
        return _myData;
    };
    
    bool empty() {
        return _myEmpty;
    };
    
    void clear() {
        std::cerr << "XXX Buffer Port incomplete." << std::endl;
    };
    
    void put(float theValue) {
        if (_myCurrentIndex < _mySize) {
            _myData[_myCurrentIndex] = theValue;
            _myCurrentIndex++;
        } else {
            std::cerr << "Buffer out of bounds" << std::endl;
        }
    };
    
    void rewind() {
        _myCurrentIndex = 0;
    };
    
private:
    float * _myData;
    bool _myEmpty;
    unsigned int _mySize;
    unsigned int _myCurrentIndex;
};
    
typedef std::tr1::shared_ptr<Buffer> BufferPtr;
    
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
        bool _myIsMapped;
	
        BufferPtr _myData;
        GLenum _myCurrentTarget;
        
        BufferObject(int theSize);

    public:
        static BufferObjectPtr create(int theSize=0);
	    void updateData();
        // std::vector<float> data(); // ByteBuffer
        // std::vector<float> mapBuffer();
        BufferPtr data();
        void mapBuffer(BufferPtr targetData);
    
        bool unmapBuffer();
        bool isMapped();
        GLuint id();
        void bind(GLenum theTarget);
        void unbind();
        void bufferData(int theSize, BufferPtr theData, int theUsageFrequency, int theUsageType);
        void bufferData(int theSize, BufferPtr theData);
        void bufferData();
        void bufferSubData(GLenum theTarget, int theOffset, int theSize, BufferPtr theData);
        void copyDataFromTexture(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight);
        
        virtual ~BufferObject();
    
    private: 
        GLuint glUsage(unsigned int theUsageFrequency, unsigned int theUsageType);
};

}; // namespace

#endif
