
#include <gl/shaderbuffer.h>
#include "bufferobject.h"
#include "graphics.h"

using namespace cclib;

BufferObject::BufferObject(int theSize) :
    _myBufferID(0), _mySize(theSize)
{
    _mySize = theSize;
    glGenBuffers(1, &(_myBufferID));
    _myData = Buffer::create(theSize);
    Graphics::checkError();
    
//    printf("%s\n\tmyBufferID %d\n",__PRETTY_FUNCTION__,_myBufferID);
    
    if (theSize>0) {
        bind(GL_ARRAY_BUFFER);
        bufferData(theSize, _myData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        unbind();
        updateData();
        Graphics::checkError();
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
        
    }

    Graphics::checkError();
    
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
    Graphics::checkError();
    
    // std::cerr << "BufferObject.cpp - port incomplete." << std::endl;
    if (targetData->size() != _mySize) {
        // XXX
        // std::cerr << "XXX incomplete" << std::endl;
#warning XXX Buffer port Incomplete
    }
    
    targetData->setGLMapBufferDataPtr(mappedBuffer);
}

bool 
BufferObject::unmapBuffer() {
    _myIsMapped = false;
    bool result = glUnmapBuffer(_myCurrentTarget);
    Graphics::checkError();
    return result;
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
    Graphics::checkError();
}

void
BufferObject::unbind(){
    glBindBuffer(_myCurrentTarget, 0);
    Graphics::checkError();
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

// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
// GL_INVALID_OPERATION is generated if the reserved buffer object name 0 is bound to target.

void 
BufferObject::bufferData(int theSize, BufferPtr theData, int theUsageFrequency, int theUsageType) {
    // set the data to the currentTarget GPU buffer object
    
    Graphics::checkError();
    _mySize = theSize;
    float * ptr;
    if (theData->empty()) {
        ptr = NULL;
    } else {
        ptr = theData->data();
    }
    
    GLenum usage = glUsage(theUsageFrequency, theUsageType);
    
//    printf("%s\n\tmyCurrentTarget %d, usage 0x%x, %s\n",__PRETTY_FUNCTION__,_myCurrentTarget,usage,(ptr == NULL) ? "data is empty" : "data is available");
    
    glBufferData(_myCurrentTarget, _mySize * sizeof(float), ptr, usage);
    
    Graphics::checkError();
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
BufferObject::copyDataFromTexture(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight) {
    
    
//    //printf("%s\n",__PRETTY_FUNCTION__);
    
    int myNewBufferSize = theWidth * theHeight * theShaderBuffer->numberOfChannels() * sizeof(float);

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

    theShaderBuffer->bindBuffer();
    
    // bind buffer object to pixel pack buffer
    bind(GL_PIXEL_PACK_BUFFER);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + theID);

    // read from frame buffer to buffer object
    glReadPixels(theX, theY, theWidth, theHeight, theShaderBuffer->attachment(theID)->format(), GL_FLOAT, 0);

    unbind();

    theShaderBuffer->unbindBuffer();
}

        
BufferObject::~BufferObject() {
    
    //printf("%s\n",__PRETTY_FUNCTION__);
    glDeleteBuffers(1, &_myBufferID);
    _myData->clear();
    
}


