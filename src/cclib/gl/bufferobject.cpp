
#include <gl/shadertexture.h>
#include "bufferobject.h"

using namespace cclib;

BufferObject::BufferObject(int theSize) :
    _myBufferID(0), _mySize(0)    
{
    _mySize = 0;
    glGenBuffers(1, &(_myBufferID));
    _myData = Buffer::create(theSize);
    
    if (theSize>0) {
        bind(GL_ARRAY_BUFFER);
        bufferData(theSize, _myData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        updateData();
    } 
}

BufferObjectPtr 
BufferObject::create(int theSize) {
    return BufferObjectPtr(new BufferObject(theSize)); 
}

void 
BufferObject::updateData() {
    bind(GL_ARRAY_BUFFER);

    // Initialize data store of buffer object
    mapBuffer(_myData);

    if (_myData->empty()) {
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

    // XXX
    // if(!_myData.empty()) {
    //     _myData.order(ByteOrder.nativeOrder());
    // }

    unbind();
}
	
BufferPtr
BufferObject::data(){
    if(_myData->empty()) {
        bind(GL_ARRAY_BUFFER);
        mapBuffer(_myData);
    }
    return _myData;
}

void
BufferObject::mapBuffer(BufferPtr targetData) {
    _myIsMapped = true;
   
    float * mappedBuffer = (float*) glMapBuffer(_myCurrentTarget, GL_WRITE_ONLY);
    
    // XXX test
    
   /* int i = 0;
    for(float x = -1000; x < 1000; x +=2){
        for(float y = -1000; y < 1000; y +=2){
            // std::cout << x << " : " << y << std::endl;
            // _myMesh->addColor(0, 0, 0);
            mappedBuffer[i] = x;
            mappedBuffer[i+1] = y;
            mappedBuffer[i+2] = -30;

            mappedBuffer[i+3] = x;
            mappedBuffer[i+4] = y;
            mappedBuffer[i+5] = 30;
            i += 6;
        }
    } */
    
    // XXX test over
    
    // std::cerr << "BufferObject.cpp - port incomplete." << std::endl;
    if (targetData->size() != _mySize) {
        // XXX
        std::cerr << "XXX incomplete" << std::endl;
    }
    
    targetData->setData(mappedBuffer);
}

bool 
BufferObject::unmapBuffer() {
    _myIsMapped = false;
    return glUnmapBuffer(_myCurrentTarget);
}

bool 
BufferObject::isMapped() {
    return _myIsMapped;
}

GLuint 
BufferObject::id() {
    return _myBufferID;
}

void 
BufferObject::bind(GLenum theTarget){
    _myCurrentTarget = theTarget;
    glBindBuffer(theTarget, _myBufferID);
}

void 
BufferObject::unbind(){
    glBindBuffer(_myCurrentTarget, 0);
}

GLuint 
BufferObject::glUsage(unsigned int theUsageFrequency, unsigned int theUsageType) {
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
                    return GL_STATIC_DRAW;
                case BUFFERUSAGE_READ:
                    return GL_STATIC_READ;
                case BUFFERUSAGE_COPY:
                    return GL_STATIC_COPY;
            }
        case BUFFERFREQ_DYNAMIC:
            switch (theUsageType) {
                case BUFFERUSAGE_DRAW:
                    return GL_DYNAMIC_DRAW;
                case BUFFERUSAGE_READ:
                    return GL_DYNAMIC_READ;
                case BUFFERUSAGE_COPY:
                    return GL_DYNAMIC_COPY;
            }
    }
    
    return 0;
}

void 
BufferObject::bufferData(int theSize, BufferPtr theData, int theUsageFrequency, int theUsageType) {
    // set the data to the currentTarget GPU buffer object
    _mySize = theSize * sizeof(float);
    glBufferData(_myCurrentTarget, _mySize, theData->data(), glUsage(theUsageFrequency, theUsageType));
}

void 
BufferObject::bufferData(int theSize, BufferPtr theData) {
    bufferData(theSize, theData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
}

void 
BufferObject::bufferData() {
    bufferData(_mySize, _myData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
}

void 
BufferObject::bufferSubData(GLenum theTarget, int theOffset, int theSize, BufferPtr theData) {
    _mySize = theSize;
    glBufferSubData(theTarget, theOffset, theSize, theData->data());
}

void 
BufferObject::copyDataFromTexture(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight) {
    int myNewBufferSize = theWidth * theHeight * theShaderTexture->numberOfChannels() * sizeof(float);

    if(myNewBufferSize != _mySize){
        bind(GL_ARRAY_BUFFER);	
        bufferData( myNewBufferSize,
                    _myData, // NULL,
                    BUFFERFREQ_STREAM,
                    BUFFERUSAGE_COPY
                );
        unbind();
        _mySize = myNewBufferSize;
    }

    theShaderTexture->bindBuffer();
    // bind buffer object to pixel pack buffer
    bind(GL_PIXEL_PACK_BUFFER);	
    glReadBuffer(GL_COLOR_ATTACHMENT0 + theID);

    // read from frame buffer to buffer object
    glReadPixels(theX, theY, theWidth, theHeight, theShaderTexture->format(), GL_FLOAT, 0);

    unbind();

    theShaderTexture->unbindBuffer();
}

        
BufferObject::~BufferObject() {
    glDeleteBuffers(1, &_myBufferID);
}


