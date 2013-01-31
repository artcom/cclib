
#include "mesh.h"

using namespace cclib;

Mesh::Mesh(GLenum theDrawMode, int theNumberOfVertices) :
    _myNumberOfVertices(theNumberOfVertices), 
    // _myVertexSize()
    _myNumberOfIndices(0),
    _myVertices(),
    _myNormals(),
    _myColors(),
    _myIndices(),
    _myDrawMode(GL_TRIANGLES)
{
    _myDrawMode = theDrawMode;
    _myNumberOfVertices = theNumberOfVertices;
    _myVertexSize = 3;
}

MeshPtr 
Mesh::create(GLenum theDrawMode, int theNumberOfVertices) {
    return MeshPtr(new Mesh(theDrawMode, theNumberOfVertices));
}

void 
Mesh::prepareVertexData(int theNumberOfVertices, int theVertexSize) {
    _myNumberOfVertices = theNumberOfVertices;
    _myVertexSize = theVertexSize;

    if(_myVertices.empty() || _myVertices.size() / _myVertexSize != _myNumberOfVertices) {
        _myNumberOfVertices = theNumberOfVertices;
        _myVertexSize = theVertexSize;
        _myVertices = std::vector<float>(_myNumberOfVertices * _myVertexSize, 0.0f);
    }
}

void 
Mesh::prepareVertexData(int theVertexSize) {
    prepareVertexData(_myNumberOfVertices, theVertexSize);
}

void 
Mesh::addVertex(float theX, float theY, float theZ) {
    prepareVertexData(_myNumberOfVertices, 3);
    _myVertices.push_back(theX);
    _myVertices.push_back(theY);
    _myVertices.push_back(theZ);
}

void 
Mesh::addVertex(float theX, float theY, float theZ, float theW) {
    prepareVertexData(_myNumberOfVertices, 4);

    _myVertices.push_back(theX);
    _myVertices.push_back(theY);
    _myVertices.push_back(theZ);
    _myVertices.push_back(theW);
}

std::vector<float> 
Mesh::vertices() {
    return _myVertices;
}

void 
Mesh::vertices(std::vector<float> theVertices, int theVertexSize) {
    _myNumberOfVertices = theVertices.size() / theVertexSize;
    _myVertexSize = theVertexSize;
    _myVertices = theVertices;
}

void 
Mesh::clearVertices() {
    _myVertices.clear();
}

void 
Mesh::clearTextureCoords() {
    for (int i=0; i<8; i++) {
        _myTextureCoords[i].clear();
    }
}

void 
Mesh::clearNormals() {
    _myNormals.clear();
}

void 
Mesh::clearColors() {
    _myColors.clear();
}

void 
Mesh::clearIndices() {
    _myIndices.clear();
}

void 
Mesh::clearAll() {
    clearVertices();
    clearTextureCoords();
    clearNormals();
    clearColors();
    clearIndices();
}

void 
Mesh::drawMode(GLenum theDrawMode) {
    _myDrawMode = theDrawMode;
}

void 
Mesh::enable() {
    // Enable Pointers
    if(!_myVertices.empty()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(_myVertexSize, GL_FLOAT, 0, &(_myVertices[0]));
    }

    if(!_myNormals.empty()) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, &(_myNormals[0]));
    }

    for(int i = 0; i < 8; i++) {
        if(_myTextureCoords[i].empty()) {
            glClientActiveTexture(GL_TEXTURE0 + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(_myTextureCoordSize[i], GL_FLOAT, 0, &(_myTextureCoords[i][0]));
        }
    }

    if(!_myColors.empty()) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, &(_myColors[0]));
    }
}

void 
Mesh::disable() {

    // Disable Pointers
    if(!_myVertices.empty()) {
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    if(!_myNormals.empty()) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    for(int i = 0; i < 8; i++) {
        if(!_myTextureCoords[i].empty()) {
            glClientActiveTexture(GL_TEXTURE0 + i);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    glClientActiveTexture(GL_TEXTURE0);
    if(!_myColors.size()){
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void 
Mesh::drawArray() {
    // Draw All Of The Triangles At Once
    if (!_myIndices.empty()) {
        glDrawArrays(_myDrawMode, 0, _myNumberOfVertices);
    } else {
        glDrawElements(_myDrawMode, _myNumberOfIndices, GL_UNSIGNED_INT, &(_myIndices[0]));
    }
}

void 
Mesh::draw() {
    enable();
    drawArray();
    disable();
}

void 
Mesh::prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize) {
    _myNumberOfVertices = theNumberOfVertices;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;

    if(_myTextureCoords[theLevel].empty() || _myNumberOfVertices != _myTextureCoords[theLevel].size() / theTextureCoordSize) {
        _myTextureCoords[theLevel] = std::vector<float>();  
    }
}

void 
Mesh::prepareTextureCoordData(int theLevel, int theTextureCoordSize){
    prepareTextureCoordData(_myNumberOfVertices, theLevel, theTextureCoordSize);
}

void 
Mesh::addTextureCoords(int theLevel, float theX, float theY) {
    prepareTextureCoordData(_myNumberOfVertices, theLevel, 2);

    _myTextureCoords[theLevel].push_back(theX);
    _myTextureCoords[theLevel].push_back(theY);
}

void 
Mesh::addTextureCoords(float theX, float theY) {
    addTextureCoords(0, theX, theY);
}

void 
Mesh::addTextureCoords(int theLevel, Vector2fPtr & theTextureCoords) {
    addTextureCoords(theTextureCoords->x(), theTextureCoords->y());
}

void 
Mesh::addTextureCoords(int theLevel, float theX, float theY, float theZ) {
    prepareTextureCoordData(_myNumberOfVertices, theLevel, 3);

    _myTextureCoords[theLevel].push_back(theX);
    _myTextureCoords[theLevel].push_back(theY);
    _myTextureCoords[theLevel].push_back(theZ);
}

void 
Mesh::addTextureCoords(int theLevel, Vector3fPtr theTextureCoords) {
    addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), theTextureCoords->z());
}

void 
Mesh::addTextureCoords(int theLevel, float theX, float theY, float theZ, float theW) {
    prepareTextureCoordData(_myNumberOfVertices, theLevel, 4);

    _myTextureCoords[theLevel].push_back(theX);
    _myTextureCoords[theLevel].push_back(theY);
    _myTextureCoords[theLevel].push_back(theZ);
    _myTextureCoords[theLevel].push_back(theW);
}

// void addTextureCoords(int theLevel, Vector4fPtr theTextureCoords) {
//     addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), theTextureCoords->z(), theTextureCoords->w());
// }

void 
Mesh::textureCoords(int theLevel, std::vector<float> & theTextureCoords, int theTextureCoordSize) {
    _myNumberOfVertices = theTextureCoords.size() / theTextureCoordSize;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;
    _myTextureCoords[theLevel] = theTextureCoords;

    // prepareTextureCoordData(theTextureCoords.limit() / theTextureCoordSize, theLevel, theTextureCoordSize);
    // 
    // _myTextureCoords[theLevel].rewind();
    // _myTextureCoords[theLevel].put(theTextureCoords);
    // _myTextureCoords[theLevel].rewind();
}

void 
Mesh::textureCoords(int theLevel, std::vector<float> & theTextureCoords) {
    textureCoords(theLevel, theTextureCoords, 2);
}

void 
Mesh::textureCoords(std::vector<float> & theTextureCoords) {
    textureCoords(0, theTextureCoords);
}

// void textureCoords(final int theLevel, final List<?> theTextureCoords){
//     if(theTextureCoords.get(0) instanceof CCVector2f) {
//         prepareTextureCoordData(theTextureCoords.size(), theLevel, 2);
//         _myTextureCoords[theLevel].rewind();
//         for(Object myObject:theTextureCoords){
//             CCVector2f myTextureCoords = (CCVector2f)myObject;
//             _myTextureCoords[theLevel].put(myTextureCoords.x);
//             _myTextureCoords[theLevel].put(myTextureCoords.y);
//         }
//     }else if(theTextureCoords.get(0) instanceof CCVector4f) {
//         prepareTextureCoordData(theTextureCoords.size(), theLevel, 4);
//         _myTextureCoords[theLevel].rewind();
//         for(Object myObject:theTextureCoords){
//             CCVector4f myTextureCoords = (CCVector4f)myObject;
//             _myTextureCoords[theLevel].put(myTextureCoords.x);
//             _myTextureCoords[theLevel].put(myTextureCoords.y);
//             _myTextureCoords[theLevel].put(myTextureCoords.z);
//             _myTextureCoords[theLevel].put(myTextureCoords.w);
//         }
//     }else if(theTextureCoords.get(0) instanceof CCVector3f) {
//         prepareTextureCoordData(theTextureCoords.size(), theLevel, 3);
//         _myTextureCoords[theLevel].rewind();
//         for(Object myObject:theTextureCoords){
//             CCVector3f myTextureCoords = (CCVector3f)myObject;
//             _myTextureCoords[theLevel].put(myTextureCoords.x);
//             _myTextureCoords[theLevel].put(myTextureCoords.y);
//             _myTextureCoords[theLevel].put(myTextureCoords.z);
//         }
//     }
// }
//
// void textureCoords(List<?> theTextureCoords){
//     textureCoords(0, theTextureCoords);
// }

std::vector<float> 
Mesh::texCoords(int theLevel) {
    return _myTextureCoords[theLevel];
}


