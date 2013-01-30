
#include <gl/shadertexture.h>
#include <gl/mesh.h>
#include <gl/bufferobject.h>
#include "vbomesh.h"

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
{}
        
VBOMeshPtr 
VBOMesh::create(GLenum theDrawMode, int theNumberOfVertices) {
    return VBOMeshPtr(new VBOMesh(theDrawMode, theNumberOfVertices));
}
    
void 
VBOMesh::bufferSubData(GLuint theBufferID, std::vector<float> & theData, int theOffset, 
        int theNumberOfVertices, int theNumberOfCoords) 
{
    // XXX ??? theData.flip();
    // Bind The Buffer
    glBindBuffer(GL_ARRAY_BUFFER, theBufferID);

    // Load The Data
    glBufferSubData(GL_ARRAY_BUFFER, theOffset, 
            theNumberOfVertices * theNumberOfCoords * sizeof(float), &(theData[0]));
}

//////////////////////////////////////////////////////
//
//  METHODS TO ADD VERTEX DATA
//
//////////////////////////////////////////////////////

void 
VBOMesh::prepareVertexData(int theNumberOfVertices, int theVertexSize) {
    _myNumberOfVertices = theNumberOfVertices;
    _myVertexSize = theVertexSize;

    if(_myVertices.empty() || _myVertices.size() / _myVertexSize != _myNumberOfVertices) {
        _myVertexSize = theVertexSize;
        
        if(!_myVertexBuffer) {
            _myVertexBuffer = BufferObject::create(_myNumberOfVertices * _myVertexSize * sizeof(float));
        } else {
            std::vector<float> noData;
            _myVertexBuffer->bufferData(_myNumberOfVertices * _myVertexSize * sizeof(float), 
                    noData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        }

        _myVertices = _myVertexBuffer->data();
    }
    
    _myHasUpdatedVertices = true;
    _myHasVertices = true;
}

void 
VBOMesh::vertices(ShaderTexturePtr theShaderTexture) {
    vertices(theShaderTexture, 0, 0, theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::vertices(ShaderTexturePtr theShaderTexture, GLuint theID) {
    vertices(theShaderTexture, 0, 0, 0, theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::vertices(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight) {
    vertices(theShaderTexture, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::vertices(ShaderTexturePtr theShaderTexture, GLuint theID, int theX, int theY, int theWidth, int theHeight) {
    if(!_myVertexBuffer) {
        _myVertexBuffer = BufferObject::create();
    }

    _myVertexSize = theShaderTexture->numberOfChannels();
    _myNumberOfVertices = theWidth * theHeight;
    
    _myVertexBuffer->copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);
    
    _myHasVertices = true;
    _myHasUpdatedVertices = false;
}

BufferObjectPtr 
VBOMesh::vertexBuffer() {
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
    _myNumberOfVertices = theNumberOfVertices;

    if(!_myNormalBuffer || _myNormals.size() / 3 != _myNumberOfVertices) {
        _myNumberOfVertices = theNumberOfVertices;
        _myNormalBuffer = BufferObject::create(_myNumberOfVertices * 3 * sizeof(float));
        _myNormals = _myNormalBuffer->data();
    }
    
    _myHasNormals = true;
    _myHasUpdatedNormals = true;
}

void 
VBOMesh::normals(ShaderTexturePtr theShaderTexture){
    normals(theShaderTexture,0,0,theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::normals(ShaderTexturePtr theShaderTexture, int theID){
    normals(theShaderTexture,theID,0,0,theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::normals(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight) {
    normals(theShaderTexture, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::normals(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight) {    	
    if (!_myNormalBuffer) {
        _myNormalBuffer = BufferObject::create();
    }

    _myNormalBuffer->copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);

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
    _myNumberOfVertices = theNumberOfVertices;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;
    
    if(!_myTextureCoordBuffers[theLevel] || _myTextureCoords[theLevel].size() / _myTextureCoordSize[theLevel] != _myNumberOfVertices) {
        if (!_myTextureCoordBuffers[theLevel]) {
            _myTextureCoordBuffers[theLevel] = BufferObject::create(_myNumberOfVertices * theTextureCoordSize * sizeof(float));
        } else {
            std::vector<float> noData;
            _myTextureCoordBuffers[theLevel]->bufferData(_myNumberOfVertices * theTextureCoordSize * sizeof(float), 
                    noData, BUFFERFREQ_DYNAMIC, BUFFERUSAGE_DRAW);
        }

        _myTextureCoords[theLevel] = _myTextureCoordBuffers[theLevel]->data();
    }

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
    _myNumberOfVertices = theNumberOfVertices;
    
    if(!_myColorBuffer || _myColors.size() / 4 != _myNumberOfVertices){
        _myColorBuffer = BufferObject::create(_myNumberOfVertices * 4 * sizeof(float));
        _myColors = _myColorBuffer->data();
    }
    
    _myHasColors = true;
    _myHasUpdatedColors = true;
}

void 
VBOMesh::colors(ShaderTexturePtr theShaderTexture){
    colors(theShaderTexture,0,0,theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::colors(ShaderTexturePtr theShaderTexture, int theID){
    colors(theShaderTexture,theID,0,0,theShaderTexture->width(), theShaderTexture->height());
}

void 
VBOMesh::colors(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight) {
    colors(theShaderTexture, 0, theX, theY, theWidth, theHeight);
}

void 
VBOMesh::colors(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight) {
    if(!_myColorBuffer){
        _myColorBuffer = BufferObject::create(theWidth * theHeight * sizeof(float));
    }
    
    _myColorBuffer->copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);
    
    _myHasColors = true;
    _myHasUpdatedColors = false;
}

void 
VBOMesh::enable(){
    
    // Enable Pointers
    for(int i = 0; i < 8; i++) {
        if(_myHasTextureCoords[i]){
            if(_myHasUpdatedTextureCoords[i]) {
                _myTextureCoordBuffers[i]->bind(GL_ARRAY_BUFFER);
                _myTextureCoordBuffers[i]->bufferData();
                _myTextureCoordBuffers[i]->unbind();
                _myHasUpdatedTextureCoords[i] = false;
            }
            _myTextureCoordBuffers[i]->bind(GL_ARRAY_BUFFER);
            glClientActiveTexture(GL_TEXTURE0 + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(_myTextureCoordSize[i], GL_FLOAT, 0, 0);
        }
    }

    if(_myHasColors){
        if(_myHasUpdatedColors) {
            _myColorBuffer->bind(GL_ARRAY_BUFFER);
            _myColorBuffer->bufferData();
            _myColorBuffer->unbind();
            _myHasUpdatedColors = false;
        }
        _myColorBuffer->bind(GL_ARRAY_BUFFER);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, 0);
    }
    if(_myHasNormals){
        if(_myHasUpdatedNormals) {
            _myNormalBuffer->bind(GL_ARRAY_BUFFER);
            _myNormalBuffer->bufferData();
            _myNormalBuffer->unbind();
            _myHasUpdatedNormals = false;
        }
        _myNormalBuffer->bind(GL_ARRAY_BUFFER);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
    }
    if(_myHasVertices){
        if(_myHasUpdatedVertices) {
            _myVertexBuffer->bind(GL_ARRAY_BUFFER);
            _myVertexBuffer->bufferData();
            _myVertexBuffer->unbind();
            _myHasUpdatedVertices = false;
        }
        _myVertexBuffer->bind(GL_ARRAY_BUFFER);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(_myVertexSize, GL_FLOAT, 0, 0);
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

