#ifndef __CCLIB_MESH_INCLUDED__
#define __CCLIB_MESH_INCLUDED__

#include <cclib.h>

namespace cclib {

class Mesh {
  
    protected:
        int _myNumberOfVertices; // = 0;
        int _myVertexSize;
        int _myTextureCoordSize[8]; //  = new int[8];
        
        int _myNumberOfIndices; // = 0;
        
        std::vector<float> _myVertices;
        std::vector<float> _myNormals;
        std::vector<float> _myTextureCoords[8]; // = new FloatBuffer[8];
        std::vector<float> _myColors;
        
        std::vector<int> _myIndices;
        GLenum _myDrawMode; // = CCDrawMode.TRIANGLES;
        Mesh(GLenum theDrawMode, int theNumberOfVertices=0);
    
    public:
        static MeshPtr create(GLenum theDrawMode, int theNumberOfVertices=0);

        void prepareVertexData(int theNumberOfVertices, int theVertexSize);
        void prepareVertexData(int theVertexSize);
        void addVertex(float theX, float theY, float theZ=0.0f);
        void addVertex(float theX, float theY, float theZ, float theW);
        std::vector<float> vertices();
        void vertices(std::vector<float> theVertices, int theVertexSize);
        void clearVertices();
        void clearTextureCoords();
        void clearNormals();
        void clearColors();
        void clearIndices();
        void clearAll();
        void drawMode(GLenum theDrawMode);
        void enable();
        void disable();
        void drawArray();
        void draw();
};
}; // namespace

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

#endif
