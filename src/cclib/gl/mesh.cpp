
#include "mesh.h"
#include <gl/texture2d.h>
#include <gl/graphics.h>

using namespace cclib;

Mesh::Mesh(GLenum theDrawMode, int theNumberOfVertices) :
    _myNumberOfVertices(theNumberOfVertices), 
    // _myVertexSize()
    _myNumberOfIndices(0),
    _myDrawMode(GL_TRIANGLES),
    _myVerticesIdx(0),
    _myNormalsIdx(0),
    _myColorsIdx(0)
{
    _myVertices = Buffer::create(0);
    _myNormals  = Buffer::create(0);
    _myColors   = Buffer::create(0);
    // _myIndices  = BufferPtr(new Buffer(0));

    for (unsigned int i=0; i<8; i++) {
        _myTextureCoords[i] = Buffer::create(0);
    }
    
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

    if(_myVertices->empty() || _myVertices->size() / _myVertexSize != _myNumberOfVertices)
    {
        _myVertices = Buffer::create(_myNumberOfVertices * _myVertexSize);
        _myVerticesIdx = 0;
    }
}

void 
Mesh::prepareVertexData(int theVertexSize) {
    prepareVertexData(_myNumberOfVertices, theVertexSize);
}

void 
Mesh::prepareColorData(int theNumberOfVertices) {
    _myNumberOfVertices = theNumberOfVertices;
    if (_myColors->empty() || _myColors->size() / 4 != _myNumberOfVertices){
        _myNumberOfVertices = theNumberOfVertices;
        _myColors = Buffer::create(_myNumberOfVertices * 4);
    }
}

void 
Mesh::addVertex(float theX, float theY, float theZ) {
    prepareVertexData(_myNumberOfVertices, 3);
    _myVertices->data()[_myVerticesIdx]   = theX;
    _myVertices->data()[_myVerticesIdx+1] = theY;
    _myVertices->data()[_myVerticesIdx+2] = theZ;
    _myVerticesIdx +=3;
}

void 
Mesh::addVertex(float theX, float theY, float theZ, float theW) {
    prepareVertexData(_myNumberOfVertices, 4);
    _myVertices->data()[_myVerticesIdx]   = theX;
    _myVertices->data()[_myVerticesIdx+1] = theY;
    _myVertices->data()[_myVerticesIdx+2] = theZ;
    _myVertices->data()[_myVerticesIdx+3] = theW;
    _myVerticesIdx +=4;
}

BufferPtr
Mesh::vertices() {
    return _myVertices;
}

void 
Mesh::vertices(BufferPtr theVertices, int theVertexSize) {
    _myNumberOfVertices = theVertices->size() / theVertexSize;
    _myVertexSize = theVertexSize;
    _myVertices = theVertices;
    theVertices->rewind();
}

void 
Mesh::clearVertices() {
    _myVertices->clear();
}

void 
Mesh::clearTextureCoords() {
    for (int i=0; i<8; i++) {
        _myTextureCoords[i]->clear();
    }
}

void 
Mesh::clearNormals() {
    _myNormals->clear();
}

void 
Mesh::clearColors() {
    _myColors->clear();
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
    if(!_myVertices->empty()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(_myVertexSize, GL_FLOAT, 0, _myVertices->data());
    }
    Graphics::checkError();
    
    if(!_myNormals->empty()) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, _myNormals->data());
    }
    Graphics::checkError();

    for(int i = 0; i < 8; i++) {
        if(!_myTextureCoords[i]->empty()) {
            glClientActiveTexture(GL_TEXTURE0 + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(_myTextureCoordSize[i], GL_FLOAT, 0, _myTextureCoords[i]->data());
            Graphics::checkError();
        }
    }

    if(!_myColors->empty()) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, _myColors->data());
        Graphics::checkError();
    }
}

void 
Mesh::disable() {

    // Disable Pointers
    if(!_myVertices->empty())
    {
        glDisableClientState(GL_VERTEX_ARRAY);
        Graphics::checkError();
    }

    if(!_myNormals->empty())
    {
        glDisableClientState(GL_NORMAL_ARRAY);
        Graphics::checkError();
    }

    for(int i = 0; i < 8; i++) {
        if(!_myTextureCoords[i]->empty())
        {
            glClientActiveTexture(GL_TEXTURE0 + i);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            Graphics::checkError();
        }
    }

    glClientActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
//    glDisable(GL_TEXTURE_RECTANGLE);
    
    if(!_myColors->empty())
    {
        glDisableClientState(GL_COLOR_ARRAY);
        Graphics::checkError();
    }
}

void 
Mesh::drawArray() {
    // Draw All Of The Triangles At Once
    if (_myIndices.empty()) {
        glDrawArrays(_myDrawMode, 0, _myNumberOfVertices);
        Graphics::checkError();
    } else {
        glDrawElements(_myDrawMode, _myNumberOfIndices, GL_UNSIGNED_INT, 0); //&(_myIndices[0]));
        Graphics::checkError();
    }
}

void 
Mesh::draw() {
    enable();
    Graphics::checkError();
    drawArray();
    Graphics::checkError();
    disable();
}

void 
Mesh::prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize) {
    _myNumberOfVertices = theNumberOfVertices;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;

    printf("%s\n\tlevel %d, size %d\n",__PRETTY_FUNCTION__,theLevel,_myTextureCoords[theLevel]->size());
    
    if(_myTextureCoords[theLevel]->empty() || _myNumberOfVertices != _myTextureCoords[theLevel]->size() / theTextureCoordSize) {
//        _myTextureCoords[theLevel] = std::vector<float>();
    }
}

void 
Mesh::prepareTextureCoordData(int theLevel, int theTextureCoordSize){
    prepareTextureCoordData(_myNumberOfVertices, theLevel, theTextureCoordSize);
}

void 
Mesh::addTextureCoords(int theLevel, float theX, float theY) {
    prepareTextureCoordData(_myNumberOfVertices, theLevel, 2);

    _myTextureCoords[theLevel]->put(theX);
    _myTextureCoords[theLevel]->put(theY);
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

    _myTextureCoords[theLevel]->put(theX);
    _myTextureCoords[theLevel]->put(theY);
    _myTextureCoords[theLevel]->put(theZ);
}

void 
Mesh::addTextureCoords(int theLevel, Vector3fPtr theTextureCoords) {
    addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), theTextureCoords->z());
}

void 
Mesh::addTextureCoords(int theLevel, float theX, float theY, float theZ, float theW) {
    prepareTextureCoordData(_myNumberOfVertices, theLevel, 4);

    _myTextureCoords[theLevel]->put(theX);
    _myTextureCoords[theLevel]->put(theY);
    _myTextureCoords[theLevel]->put(theZ);
    _myTextureCoords[theLevel]->put(theW);
}
        
void 
Mesh::addColor(float theRed, float theGreen, float theBlue, float theAlpha){
    prepareColorData(_myNumberOfVertices);
    _myColors->put(theRed);
    _myColors->put(theGreen);
    _myColors->put(theBlue);
    _myColors->put(theAlpha);
}

// void 
// Mesh::addColor(Color & theColor){
//     float r = theColor.red();
//     
//     addColor(theColor.red(), theColor.green(), theColor.blue(), theColor.alpha());
// }

void 
Mesh::addColor(float theRed, float theGreen, float theBlue){
    addColor(theRed, theGreen, theBlue, 1.0f);
}

void 
Mesh::addColor(float theGray, float theAlpha){
    addColor(theGray, theGray, theGray, theAlpha);
}

void 
Mesh::addColor(float theGray){
    addColor(theGray, theGray, theGray, 1.0f);
}

// void 
// Mesh::colors(const std::vector<Color> & theColors) {
//     prepareColorData(theColors.size());
//     _myColors->rewind();
// 
//     for(unsigned int i=0; i<theColors.size(); i++){
//         _myColors->put(theColors[i].red());
//         _myColors->put(theColors[i].green());
//         _myColors->put(theColors[i].blue());
//         _myColors->put(theColors[i].alpha());
//     }
//     _myColors->rewind();
// }

void 
Mesh::colors(BufferPtr theColors) {
    prepareColorData(theColors->size() / 4);
    _myColors->rewind();
    _myColors->put(theColors);
    _myColors->rewind();
}

// void addTextureCoords(int theLevel, Vector4fPtr theTextureCoords) {
//     addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), theTextureCoords->z(), theTextureCoords->w());
// }

void 
Mesh::textureCoords(int theLevel, BufferPtr theTextureCoords, int theTextureCoordSize) {
    _myNumberOfVertices = theTextureCoords->size() / theTextureCoordSize;
    _myTextureCoordSize[theLevel] = theTextureCoordSize;
    _myTextureCoords[theLevel] = theTextureCoords;
    theTextureCoords->rewind();
}

void 
Mesh::textureCoords(int theLevel, BufferPtr theTextureCoords) {
    textureCoords(theLevel, theTextureCoords, 2);
}

void 
Mesh::textureCoords(BufferPtr theTextureCoords) {
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

int
Mesh::numberOfVertices() {
    return _myNumberOfVertices;
}

BufferPtr
Mesh::texCoords(int theLevel) {
    return _myTextureCoords[theLevel];
}


