#ifndef __CCLIB_MESH_INCLUDED__
#define __CCLIB_MESH_INCLUDED__

#include <cclib.h>
#include <gl/bufferobject.h>

namespace cclib {

class Mesh {
  
    protected:
        int _myNumberOfVertices; // = 0;
        int _myVertexSize;
        int _myTextureCoordSize[8]; //  = new int[8];
        
        int _myNumberOfIndices; // = 0;
        
        BufferPtr _myVertices;
        BufferPtr _myNormals;
        BufferPtr _myTextureCoords[8]; // = new FloatBuffer[8];
        BufferPtr _myColors;

        int _myVerticesIdx;
        int _myNormalsIdx;
        int _myTextureCoordsIdx[8];
        int _myColorsIdx;
    
        std::vector<int> _myIndices;
        GLenum _myDrawMode; // = CCDrawMode.TRIANGLES;
        Mesh(GLenum theDrawMode, int theNumberOfVertices=0);
    
    public:
        static MeshPtr create(GLenum theDrawMode, int theNumberOfVertices=0);

        virtual void prepareVertexData(int theNumberOfVertices, int theVertexSize);
        virtual void prepareVertexData(int theVertexSize);
        virtual void addVertex(float theX, float theY, float theZ=0.0f);
        virtual void addVertex(float theX, float theY, float theZ, float theW);
        virtual BufferPtr vertices();
        virtual void vertices(BufferPtr theVertices, int theVertexSize=3);
        virtual void clearVertices();
        virtual void clearTextureCoords();
        virtual void clearNormals();
        virtual void clearColors();
        virtual void clearIndices();
        virtual void clearAll();
        virtual void drawMode(GLenum theDrawMode);
        virtual void enable();
        virtual void disable();
        virtual void drawArray();
        virtual void draw();
	
        virtual void prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize);
        virtual void prepareTextureCoordData(int theLevel, int theTextureCoordSize);
        virtual void addTextureCoords(int theLevel, float theX, float theY);
        virtual void addTextureCoords(float theX, float theY);
        virtual void addTextureCoords(int theLevel, Vector2fPtr & theTextureCoords);
        virtual void addTextureCoords(int theLevel, float theX, float theY, float theZ);
        virtual void addTextureCoords(int theLevel, Vector3fPtr theTextureCoords);
        virtual void addTextureCoords(int theLevel, float theX, float theY, float theZ, float theW);
        
        virtual void textureCoords(int theLevel, BufferPtr theTextureCoords, int theTextureCoordSize);
        virtual void textureCoords(int theLevel, BufferPtr theTextureCoords);
        virtual void textureCoords(BufferPtr theTextureCoords);
        virtual BufferPtr texCoords(int theLevel);
        
        // void addTextureCoords(int theLevel, Vector4fPtr theTextureCoords) {
        //     addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), theTextureCoords->z(), theTextureCoords->w());
        // }
        
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

};
}; // namespace

    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD TEXTURE COORD DATA
    //
    //////////////////////////////////////////////////////
	
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
