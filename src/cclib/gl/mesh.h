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
    
        virtual void prepareColorData(int theNumberOfVertices);
        virtual void addColor(float theRed, float theGreen, float theBlue, float theAlpha);
        // virtual void addColor(Color & theColor);
        virtual void addColor(float theRed, float theGreen, float theBlue);
        virtual void addColor(float theGray, float theAlpha);
        virtual void addColor(float theGray);
        // virtual void colors(const std::vector<Color> & theColors);
        virtual void colors(BufferPtr theColors);
            
        // void addTextureCoords(int theLevel, Vector4fPtr theTextureCoords) {
        //     addTextureCoords(theLevel, theTextureCoords->x(), theTextureCoords->y(), 
        //             theTextureCoords->z(), theTextureCoords->w());
        // }

        // void textureCoords(int theLevel, List<?> theTextureCoords){
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

        // void textureCoords(List<?> theTextureCoords){
        //     textureCoords(0, theTextureCoords);
        // }
        // 
        // 
        // void indices(List<Integer> theIndices){
        //     if(theIndices.size() == 0)return;
        //     _myNumberOfIndices = theIndices.size();
        //     _myIndices = CCBufferUtil.newIntBuffer(theIndices.size());
        //     for(int myIndex:theIndices){
        //         _myIndices.put(myIndex);
        //     }
        //     _myIndices.rewind();
        // }
        // 
        // void indices(int[] theIndices) {
        //     indices(IntBuffer.wrap(theIndices));
        // }
        // 
        // void indices(IntBuffer theIndices) {
        //     _myNumberOfIndices = theIndices.capacity();
        //     if(theIndices.hasArray()) {
        //         _myIndices = theIndices;
        //     }else {
        //         _myIndices = CCBufferUtil.newIntBuffer(theIndices.capacity());
        //         theIndices.rewind();
        //         _myIndices.put(theIndices);
        //     }
        //     _myIndices.rewind();
        // }
        // 
        // void noIndices() {
        //     _myIndices = null;
        // }
        // 
        // IntBuffer indices() {
        //     return _myIndices;
        // }

        // int numberOfVertices() {
        //     return _myNumberOfVertices;
        // }
};
}; // namespace
    


    //////////////////////////////////////////////////////
    //
    //  METHODS TO RESET THE MESH
    //
    //////////////////////////////////////////////////////

#endif
