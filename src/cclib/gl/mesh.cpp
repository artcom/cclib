
#include "mesh.h"

using namespace cclib;

Mesh::Mesh(GLenum theDrawMode, int theNumberOfVertices) :
    _myNumberOfVertices(theNumberOfVertices), 
    // _myVertexSize()
    _myTextureCoordSize(),
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

    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD TEXTURE COORD DATA
    //
    //////////////////////////////////////////////////////
	
	// public void prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize){
	// 	_myNumberOfVertices = theNumberOfVertices;
	// 	_myTextureCoordSize[theLevel] = theTextureCoordSize;
	// 	
	// 	if(_myTextureCoords[theLevel] == null || _myNumberOfVertices != _myTextureCoords[theLevel].limit() / theTextureCoordSize) {
    // 		_myTextureCoords[theLevel] = CCBufferUtil.newDirectFloatBuffer(_myNumberOfVertices * theTextureCoordSize);
    // 	}
	// }
	// public void prepareTextureCoordData(int theLevel, int theTextureCoordSize){
	// 	prepareTextureCoordData(_myNumberOfVertices, theLevel, theTextureCoordSize);
	// }
    // 
    // public void addTextureCoords(final int theLevel, final float theX, final float theY){
    // 	prepareTextureCoordData(_myNumberOfVertices, theLevel, 2);
    // 	
    // 	_myTextureCoords[theLevel].put(theX);
    // 	_myTextureCoords[theLevel].put(theY);
    // }
    // 
    // public void addTextureCoords(final float theX, final float theY){
    // 	addTextureCoords(0, theX, theY);
    // }
    // 
    // public void addTextureCoords(final int theLevel, final CCVector2f theTextureCoords){
    // 	addTextureCoords(theTextureCoords.x, theTextureCoords.y);
    // }
    // 
    // public void addTextureCoords(final int theLevel, final float theX, final float theY, final float theZ){
    // 	prepareTextureCoordData(_myNumberOfVertices, theLevel, 3);
    // 	
    // 	_myTextureCoords[theLevel].put(theX);
    // 	_myTextureCoords[theLevel].put(theY);
    // 	_myTextureCoords[theLevel].put(theZ);
    // }
    // 
    // public void addTextureCoords(final int theLevel, final CCVector3f theTextureCoords){
    // 	addTextureCoords(theLevel, theTextureCoords.x, theTextureCoords.y, theTextureCoords.z);
    // }
    // 
    // public void addTextureCoords(final int theLevel, final float theX, final float theY, final float theZ, final float theW){
    // 	prepareTextureCoordData(_myNumberOfVertices, theLevel, 4);
    // 	
    // 	_myTextureCoords[theLevel].put(theX);
    // 	_myTextureCoords[theLevel].put(theY);
    // 	_myTextureCoords[theLevel].put(theZ);
    // 	_myTextureCoords[theLevel].put(theW);
    // }
    // 
    // public void addTextureCoords(final int theLevel, final CCVector4f theTextureCoords){
    // 	addTextureCoords(theLevel, theTextureCoords.x, theTextureCoords.y, theTextureCoords.z, theTextureCoords.w);
    // }
    // 
    // public void textureCoords(final int theLevel, final FloatBuffer theTextureCoords, final int theTextureCoordSize){
    // 	_myNumberOfVertices = theTextureCoords.limit() / theTextureCoordSize;
	// 	_myTextureCoordSize[theLevel] = theTextureCoordSize;
    // 	_myTextureCoords[theLevel] = theTextureCoords;
    // 	_myTextureCoords[theLevel].rewind();
//  //   	prepareTextureCoordData(theTextureCoords.limit() / theTextureCoordSize, theLevel, theTextureCoordSize);
//  //   	
//  //   	_myTextureCoords[theLevel].rewind();
//  //   	_myTextureCoords[theLevel].put(theTextureCoords);
//  //   	_myTextureCoords[theLevel].rewind();
    // }
    // 
    // public void textureCoords(final int theLevel, final FloatBuffer theTextureCoords){
    // 	textureCoords(theLevel, theTextureCoords, 2);
    // }
    // 
    // public void textureCoords(final FloatBuffer theTextureCoords){
    // 	textureCoords(0, theTextureCoords);
    // }
    // 
    // public void textureCoords(final int theLevel, final List<?> theTextureCoords){
    // 	if(theTextureCoords.get(0) instanceof CCVector2f) {
    // 		prepareTextureCoordData(theTextureCoords.size(), theLevel, 2);
    //     	_myTextureCoords[theLevel].rewind();
    //     	for(Object myObject:theTextureCoords){
    //     		CCVector2f myTextureCoords = (CCVector2f)myObject;
    //     		_myTextureCoords[theLevel].put(myTextureCoords.x);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.y);
    //     	}
    // 	}else if(theTextureCoords.get(0) instanceof CCVector4f) {
    // 		prepareTextureCoordData(theTextureCoords.size(), theLevel, 4);
    //     	_myTextureCoords[theLevel].rewind();
    //     	for(Object myObject:theTextureCoords){
    //     		CCVector4f myTextureCoords = (CCVector4f)myObject;
    //     		_myTextureCoords[theLevel].put(myTextureCoords.x);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.y);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.z);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.w);
    //     	}
    // 	}else if(theTextureCoords.get(0) instanceof CCVector3f) {
    // 		prepareTextureCoordData(theTextureCoords.size(), theLevel, 3);
    //     	_myTextureCoords[theLevel].rewind();
    //     	for(Object myObject:theTextureCoords){
    //     		CCVector3f myTextureCoords = (CCVector3f)myObject;
    //     		_myTextureCoords[theLevel].put(myTextureCoords.x);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.y);
    //     		_myTextureCoords[theLevel].put(myTextureCoords.z);
    //     	}
    // 	}
    // 	
    // 	
    // 	
    // 	_myTextureCoords[theLevel].rewind();
    // }
    // 
    // public void textureCoords(final List<?> theTextureCoords){
    // 	textureCoords(0, theTextureCoords);
    // }
    // 
    // public FloatBuffer texCoords(int theLevel) {
    // 	return _myTextureCoords[theLevel];
    // }
    
    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD COLOR DATA
    //
    //////////////////////////////////////////////////////
    
    // public void prepareColorData(int theNumberOfVertices){
    // 	_myNumberOfVertices = theNumberOfVertices;
    // 	if(_myColors == null || _myColors.limit() / 4 != _myNumberOfVertices){
    // 		_myNumberOfVertices = theNumberOfVertices;
    // 		_myColors = CCBufferUtil.newDirectFloatBuffer(_myNumberOfVertices * 4);
    // 	}
    // }
    // 
    // public void addColor(final float theRed, final float theGreen, final float theBlue, final float theAlpha){
    // 	prepareColorData(_myNumberOfVertices);
    // 	_myColors.put(theRed);
    // 	_myColors.put(theGreen);
    // 	_myColors.put(theBlue);
    // 	_myColors.put(theAlpha);
    // }
    // 
    // public void addColor(final CCColor theColor){
    // 	addColor(theColor.r, theColor.g, theColor.b, theColor.a);
    // }
    // 
    // public void addColor(final float theRed, final float theGreen, final float theBlue){
    // 	addColor(theRed, theGreen, theBlue, 1f);
    // }
    // 
    // public void addColor(final float theGray, final float theAlpha){
    // 	addColor(theGray, theGray, theGray, theAlpha);
    // }
    // 
    // public void addColor(final float theGray){
    // 	addColor(theGray, theGray, theGray, 1f);
    // }
    // 
    // public void colors(final List<CCColor> theColors){
    // 	prepareColorData(theColors.size());
    // 	_myColors.rewind();
    // 	
    // 	for(CCColor myColor:theColors){
    // 		_myColors.put(myColor.r);
    // 		_myColors.put(myColor.g);
    // 		_myColors.put(myColor.b);
    // 		_myColors.put(myColor.a);
    // 	}
    // 	_myColors.rewind();
    // }
    // 
    // public void colors(final FloatBuffer theColors){
    // 	prepareColorData(theColors.limit() / 4);
    // 	_myColors.rewind();
    // 	_myColors.put(theColors);
    // 	_myColors.rewind();
    // }
    // 
    // public void indices(final List<Integer> theIndices){
    // 	if(theIndices.size() == 0)return;
    // 	_myNumberOfIndices = theIndices.size();
    // 	_myIndices = CCBufferUtil.newIntBuffer(theIndices.size());
    // 	for(int myIndex:theIndices){
    // 		_myIndices.put(myIndex);
    // 	}
    // 	_myIndices.rewind();
    // }
    // 
    // public void indices(final int[] theIndices) {
    // 	indices(IntBuffer.wrap(theIndices));
    // }
    // 
    // public void indices(final IntBuffer theIndices) {
    // 	_myNumberOfIndices = theIndices.capacity();
    // 	if(theIndices.hasArray()) {
    //     	_myIndices = theIndices;
    // 	}else {
    //     	_myIndices = CCBufferUtil.newIntBuffer(theIndices.capacity());
    //     	theIndices.rewind();
    //     	_myIndices.put(theIndices);
    // 	}
    // 	_myIndices.rewind();
    // }
    // 
    // public void noIndices() {
    // 	_myIndices = null;
    // }
    // 
    // public IntBuffer indices() {
    // 	return _myIndices;
    // }

    // public int numberOfVertices() {
    //     return _myNumberOfVertices;
    // }
    // 


    //////////////////////////////////////////////////////
    //
    //  METHODS TO RESET THE MESH
    //
    //////////////////////////////////////////////////////
    


