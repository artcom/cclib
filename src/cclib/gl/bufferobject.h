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

class Buffer;
typedef std::tr1::shared_ptr<Buffer> BufferPtr;

// 
    
class Buffer {
public:
    Buffer(unsigned int theSize, bool theInitializedFlag) :
        _myData(NULL),
         _myIsEmpty(!theInitializedFlag),
         _mySize(theSize),
        _myCurrentIndex(0), _myIsSelfAllocated(false)
    {
//        printf("%s\n\tsize %d\n",__PRETTY_FUNCTION__, _mySize);
        
        if (_mySize == 0) {

            return;
        }
        
        _myData = new float[_mySize]();
        _myIsSelfAllocated = true;
        _myIsEmpty = false;
    };
    
    static BufferPtr create(unsigned int theSize, bool theInitializedFlag = false) {
        return BufferPtr(new Buffer(theSize, theInitializedFlag));
    };
    
    virtual ~Buffer() {
        if (_myData != NULL && _myIsSelfAllocated) {
            delete _myData;
        }
    };
    
    void setGLMapBufferDataPtr(float * theData) {
        
        if (_myData != NULL && _myIsSelfAllocated) {
            delete _myData;
            _myData = NULL;
            _myIsEmpty = true;
        }
        
        if (theData) {
            _myData = theData;
            _myIsEmpty = false;
            _myIsSelfAllocated = false;
        }
    };

    unsigned int size() {
        return _mySize;
    };
    
    float * data() {
        if (!_myData) {
            std::cerr << "Buffer is empty!" << std::endl;
            return NULL;
        }
    
        return _myData;
    };
    
    bool empty() {
        return _myIsEmpty;
    };
    
    void clear() {
#warning XXX Buffer Port incomplete.
        // std::cerr << "XXX Buffer Port incomplete." << std::endl;
        if(_myData == NULL) return;
        
        delete _myData;
        _myData = NULL;
        _myIsEmpty = true;
        _myIsSelfAllocated = false;
        _mySize = _myCurrentIndex = 0;
    };
    
    void put(float theValue) {
        
//        printf("%s\n\tcurrent %d size %d\n",__PRETTY_FUNCTION__, _myCurrentIndex, _mySize);
//        printf("current %d size %d\n", _myCurrentIndex, _mySize);
        
        if (_myCurrentIndex < _mySize) {
            _myData[_myCurrentIndex] = theValue;
            _myCurrentIndex++;
        } else {
            std::cerr << "Buffer out of bounds" << std::endl;
        }
    };
    
    void put(BufferPtr theData) {
        if (theData->size() + _myCurrentIndex > size()) {
            std::cerr << "Color->put out of bounds" << std::endl;
            return;
        }
    
        memcpy(_myData + _myCurrentIndex, theData->data(), theData->size());
        _myCurrentIndex += theData->size();
        
    };
    
    void rewind() {
        _myCurrentIndex = 0;
    };
    
private:
    bool _myIsSelfAllocated;
    float * _myData;
    bool _myIsEmpty;
    unsigned int _mySize;
    unsigned int _myCurrentIndex;
};
    
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
        void copyDataFromTexture(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight);
        
        virtual ~BufferObject();
    
    private: 
        GLuint glUsage(unsigned int theUsageFrequency, unsigned int theUsageType);
};

}; // namespace

#endif
