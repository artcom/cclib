
#include <string.h>
#include <gl/shaderbuffer.h>
#include <gl/mesh.h>
#include <gl/bufferobject.h>
#include "vbomesh.h"
#include "graphics.h"

using namespace cclib;
        

VBOMesh::VBOMesh(GLenum theDrawMode, int theNumberOfVertices) : 
    Mesh(theDrawMode, theNumberOfVertices),
    _myHasUpdatedVertices(true),
    _myHasVertices(false),
    _myHasUpdatedColors(true),
    _myHasColors(false),
    _myHasUpdatedNormals(true),
    _myHasNormals(false),
    _myHasIndices(true),
    _myHasDefinedSize(false)
{
    for (int i=0; i<8; i++) {
        _myHasTextureCoords[i] = false;
    }
}
        
VBOMeshPtr 
VBOMesh::create(GLenum theDrawMode, int theNumberOfVertices) {
    return VBOMeshPtr(new VBOMesh(theDrawMode, theNumberOfVertices));
}
    
void 
VBOMesh::bufferSubData(GLuint theBufferID, BufferPtr theData, int theOffset,
        int theNumberOfVertices, int theNumberOfCoords) 
{
    // std::cout << "vbo::bufferSubData" << std::endl;
    // XXX ??? theData.flip();
    // Bind The Buffer
    glBindBuffer(GL_ARRAY_BUFFER, theBufferID);

    // Load The Data
    glBufferSubData(GL_ARRAY_BUFFER, theOffset, 
            theNumberOfVertices * theNumberOfCoords * sizeof(float), theData->data());
}

//////////////////////////////////////////////////////
//
//  METHODS TO ADD VERTEX DATA
//
//////////////////////////////////////////////////////

void 
VBOMesh::prepareVertexData(int theNumberOfVertices, int theVertexSize) {
    // std::cout << "vbo::prepareVertexData" << std::endl;
    _myNumberOfVertices = theNumberOfVertices;
    _myVertexSize = theVertexSize;

    if(_myVertices->empty() || _myVertices->size() / _myVertexSize != _myNumberOfVertices) {
        _myVertexSize = theVertexSize;
        
        if(!_myVertexBuffer) {
            _myVertexBuffer = BufferObject::create(_myNumberOfVertices * _myVertexSize);
        } else {
            _myVertexBuffer->bufferData(_myNumberOfVertices * _myVertexSize,
                    _myVertices, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        }
        
        _myVertices = _myVertexBuffer->data();
    }
    
    _myHasUpdatedVertices = true;
    _myHasVertices = true;
}

void 
VBOMesh::vertices(ShaderBufferPtr theShaderBuffer) {
    // std::cout << "vbo::vertices/shaderbuffer" << std::endl;
    vertices(theShaderBuffer, 0, 0, theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::vertices(ShaderBufferPtr theShaderBuffer, GLuint theID) {
    // std::cout << "vbo::vertices/shaderbuffer" << std::endl;
    vertices(theShaderBuffer, 0, 0, 0, theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::vertices(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight) {
    // std::cout << "vbo::vertices/shaderbuffer" << std::endl;
    vertices(theShaderBuffer, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::vertices(ShaderBufferPtr theShaderBuffer, GLuint theID, int theX, int theY, int theWidth, int theHeight) {
    // std::cout << "vbo::vertices/shaderbuffer" << std::endl;
    if(!_myVertexBuffer) {
        _myVertexBuffer = BufferObject::create();
    }

    _myVertexSize = theShaderBuffer->numberOfChannels();
    _myNumberOfVertices = theWidth * theHeight;
    
    _myVertexBuffer->copyDataFromTexture(theShaderBuffer, theID, theX, theY, theWidth, theHeight);
    
    _myHasVertices = true;
    _myHasUpdatedVertices = false;
}

BufferObjectPtr 
VBOMesh::vertexBuffer() {
    // std::cout << "vbo::vertexBuffer" << std::endl;
    Mesh::prepareVertexData(_myVertexSize);
    return _myVertexBuffer;
}

//////////////////////////////////////////////////////
//
//  METHODS TO ADD NORMAL DATA
//
//////////////////////////////////////////////////////

void 
VBOMesh::prepareNormalData(int theNumberOfVertices){
    // std::cout << "vbo::prepareNormalData" << std::endl;
    _myNumberOfVertices = theNumberOfVertices;

    if(!_myNormalBuffer || _myNormals->size() / 3 != _myNumberOfVertices) {
        _myNumberOfVertices = theNumberOfVertices;
        _myNormalBuffer = BufferObject::create(_myNumberOfVertices * 3 * sizeof(float));
        _myNormals = _myNormalBuffer->data();
    }
    
    _myHasNormals = true;
    _myHasUpdatedNormals = true;
}

void 
VBOMesh::normals(ShaderBufferPtr theShaderBuffer){
    // std::cout << "vbo::normals" << std::endl;
    normals(theShaderBuffer,0,0,theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::normals(ShaderBufferPtr theShaderBuffer, int theID){
    // std::cout << "vbo::normals" << std::endl;
    normals(theShaderBuffer,theID,0,0,theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::normals(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight) {
    // std::cout << "vbo::normals" << std::endl;
    normals(theShaderBuffer, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::normals(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight) {    	
    // std::cout << "vbo::normals" << std::endl;
    if (!_myNormalBuffer) {
        _myNormalBuffer = BufferObject::create();
    }

    _myNormalBuffer->copyDataFromTexture(theShaderBuffer, theID, theX, theY, theWidth, theHeight);

    _myHasNormals = true;
    _myHasUpdatedNormals = false;
}

//////////////////////////////////////////////////////
//
//  METHODS TO ADD TEXTURE COORD DATA
//
//////////////////////////////////////////////////////

void 
VBOMesh::prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize){
    // std::cout << "vbo::prepareTextureCoordData" << std::endl;
    _myNumberOfVertices = theNumberOfVertices;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;
  
    if(theLevel >= _myTextureCoordBuffers.size())
    {
        printf("%s\n\tlevel %d, theNumberOfVertices %d, theTextureCoordSize %d\n",__PRETTY_FUNCTION__,theLevel,theNumberOfVertices,theTextureCoordSize);
        
        _myTextureCoordBuffers.push_back( cclib::BufferObject::create(_myNumberOfVertices * theTextureCoordSize * sizeof(float)) );
        
        _myTextureCoords[theLevel] = _myTextureCoordBuffers.back()->data();
    }
    else if(theLevel < _myTextureCoordBuffers.size())
    {
        return;
        
//        _myTextureCoordBuffers[theLevel]->bind(GL_ARRAY_BUFFER);
//        _myTextureCoordBuffers[theLevel]->bufferData(_myNumberOfVertices * theTextureCoordSize * sizeof(float),
//                                                     _myTextureCoords[theLevel], BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        
        _myTextureCoords[theLevel] = _myTextureCoordBuffers.back()->data();
    }
    
//    if(!_myTextureCoordBuffers[theLevel] || _myTextureCoords[theLevel]->size() / _myTextureCoordSize[theLevel] != _myNumberOfVertices)
//    {
//        if(!_myTextureCoordBuffers[theLevel]) {
//            
//            printf("%s\n\tlevel %d, theNumberOfVertices %d, theTextureCoordSize %d\n",__PRETTY_FUNCTION__,theLevel,theNumberOfVertices,theTextureCoordSize);
//            printf("\tcreate, size %d\n",_myTextureCoords[theLevel]->size());
//            
//            _myTextureCoordBuffers[theLevel] = cclib::BufferObject::create(_myNumberOfVertices * theTextureCoordSize * sizeof(float));
//            
//            printf("\tallocated %d\n",_myTextureCoords[theLevel]->size());
//        }
//        else {
//            
////            printf("\trealloc, size %d\n",_myTextureCoords[theLevel]->size());
////            _myTextureCoordBuffers[theLevel]->bufferData(_myNumberOfVertices * theTextureCoordSize * sizeof(float),
////                                                         _myTextureCoords[theLevel], BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
////            
////            printf("\tallocated %d\n",_myTextureCoords[theLevel]->size());
//        }
//
//        _myTextureCoords[theLevel] = _myTextureCoordBuffers[theLevel]->data();
//    }

    _myHasTextureCoords[theLevel] = true;
    _myHasUpdatedTextureCoords[theLevel] = true;
  
    
}

//////////////////////////////////////////////////
//
// METHODS TO ADD COLOR DATA
//
//////////////////////////////////////////////////

void 
VBOMesh::prepareColorData(int theNumberOfVertices){
    // std::cout << "vbo::prepareColorData" << std::endl;
     _myNumberOfVertices = theNumberOfVertices;
    
    if(_myColors->empty() || _myColors->size() / 4 != _myNumberOfVertices) {
        if (!_myColorBuffer) {
            _myColorBuffer = BufferObject::create(_myNumberOfVertices * 4 );
            _myColors = _myColorBuffer->data();
        }
    }
    
    _myHasColors = true;
    _myHasUpdatedColors = true;
}

void
VBOMesh::colors(ShaderBufferPtr theShaderBuffer){
    // std::cout << "vbo::colors" << std::endl;
    colors(theShaderBuffer, 0, 0, theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::colors(ShaderBufferPtr theShaderBuffer, int theID){
    // std::cout << "vbo::colors" << std::endl;
    colors(theShaderBuffer, theID, 0, 0, theShaderBuffer->width(), theShaderBuffer->height());
}

void 
VBOMesh::colors(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight) {
    // std::cout << "vbo::colors" << std::endl;
    colors(theShaderBuffer, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::colors(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight) {
    // std::cout << "vbo::colors" << std::endl;
    if(!_myColorBuffer){
        _myColorBuffer = BufferObject::create(theWidth * theHeight * sizeof(float));
    }
    
    _myColorBuffer->copyDataFromTexture(theShaderBuffer, theID, theX, theY, theWidth, theHeight);
    
    _myHasColors = true;
    _myHasUpdatedColors = false;
}

void 
VBOMesh::enable(){
    
#ifndef __APPLE__
#define DO_BUFFER_DATA
#endif
    
    // Enable Pointers
    for(int i = 0; i < 8; i++) {
        if(_myHasTextureCoords[i]){
            // std::cout << "vbo:textureCoords " << i << std::endl;
            if(_myHasUpdatedTextureCoords[i]) {
#ifdef DO_BUFFER_DATA
                _myTextureCoordBuffers[i]->bind(GL_ARRAY_BUFFER);
                _myTextureCoordBuffers[i]->bufferData();
                _myTextureCoordBuffers[i]->unbind();
#endif
                _myHasUpdatedTextureCoords[i] = false;
            }
            _myTextureCoordBuffers[i]->bind(GL_ARRAY_BUFFER);
            glClientActiveTexture(GL_TEXTURE0 + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(_myTextureCoordSize[i], GL_FLOAT, 0, 0);
            Graphics::checkError();
        } else {
            // std::cout << "no vbo:textureCoords " << i << std::endl;
        }
    }

    if(_myHasColors) {
        // std::cout << "vbo:color" << std::endl;
        if(_myHasUpdatedColors) {
#ifdef DO_BUFFER_DATA
        // see below.. "That block causes crashes, but if I comment it out, it works. Maybe we don't need it then? [sh]"
            _myColorBuffer->bind(GL_ARRAY_BUFFER);
            _myColorBuffer->bufferData();
            _myColorBuffer->unbind();
#endif
            _myHasUpdatedColors = false;
        }
        _myColorBuffer->bind(GL_ARRAY_BUFFER);
        Graphics::checkError();
        glEnableClientState(GL_COLOR_ARRAY);
        Graphics::checkError();
        glColorPointer(4, GL_FLOAT, 0, 0);
        Graphics::checkError();
    } else {
        // std::cout << "no vbo:colors" << std::endl;
    }
    
    if(_myHasNormals){
        // std::cout << "vbo:normal" << std::endl;
        if(_myHasUpdatedNormals) {
#ifdef DO_BUFFER_DATA
            _myNormalBuffer->bind(GL_ARRAY_BUFFER);
            _myNormalBuffer->bufferData();
            _myNormalBuffer->unbind();
#endif
            _myHasUpdatedNormals = false;
        }
        _myNormalBuffer->bind(GL_ARRAY_BUFFER);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
        Graphics::checkError();
    } else {
        // std::cout << "no vbo:normals" << std::endl;
    }
    
    if(_myHasVertices){
        // std::cout << "vbo:vertices" << std::endl;
#warning "That block causes crashes, but if I comment it out, it works. Maybe we don't need it then? [sh]"
        if(_myHasUpdatedVertices) {
#ifdef DO_BUFFER_DATA
            _myVertexBuffer->bind(GL_ARRAY_BUFFER);
            _myVertexBuffer->bufferData(_myVertices->size(), _myVertices, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
            _myVertexBuffer->unbind();
#endif 
            _myHasUpdatedVertices = false;
        }
        _myVertexBuffer->bind(GL_ARRAY_BUFFER);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(_myVertexSize, GL_FLOAT, 0, 0);
        Graphics::checkError();
    } else {
        // std::cout << "no vbo:vertices" << std::endl;
    }
    
    // if(_myHasIndices) {
    // 	
    // } // ??
}

void 
VBOMesh::disable() {
    Mesh::disable();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
VBOMesh::draw() {
    
    enable();
    Graphics::checkError();
    Mesh::drawArray();
    Graphics::checkError();
    disable();    
}


