
#ifndef __CCLIB_VBOMESH_INCLUDED__
#define __CCLIB_VBOMESH_INCLUDED__

#include <cclib.h>

namespace cclib {

class VBOMesh : public Mesh {
    
    private:
        BufferObjectPtr _myVertexBuffer;	
        bool _myHasUpdatedVertices;
        bool _myHasVertices;

        BufferObjectPtr[8] _myTextureCoordBuffers; // = new CCBufferObjectPtr[8];
        bool[8] _myHasUpdatedTextureCoords; // = new bool[] {true, true, true, true, true, true, true, true};
        bool[8] _myHasTextureCoords; // = new bool[] {false, false, false, false, false, false, false, false};

        BufferObjectPtr _myColorBuffer;
        bool _myHasUpdatedColors; 
        bool _myHasColors; 

        BufferObjectPtr _myNormalBuffer;
        bool _myHasUpdatedNormals; 
        bool _myHasNormals; 

        bool _myHasIndices; 
        bool _myHasDefinedSize;
        
        VBOMesh(GLenum theDrawMode, int theNumberOfVertices); 
    
    public:
        VBOMeshPtr create(GLenum theDrawMode=GL_QUADS, int theNumberOfVertices=0);

        void bufferSubData(GLuint theBufferID, std::vector<float> & theData, int theOffset, 
                int theNumberOfVertices, int theNumberOfCoords) 
        void prepareVertexData(int theNumberOfVertices, int theVertexSize);
        void vertices(ShaderTexturePtr theShaderTexture);
        void vertices(ShaderTexturePtr theShaderTexture, GLuint theID);
        void vertices(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight);
        void vertices(ShaderTexturePtr theShaderTexture, GLuint theID, int theX, int theY, int theWidth, int theHeight);
        BufferObjectPtr vertexBuffer();
        void prepareNormalData(int theNumberOfVertices);
        void normals(ShaderTexturePtr theShaderTexture);
        void normals(ShaderTexturePtr theShaderTexture, int theID);
        void normals(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight);
        void normals(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight);    	
        void prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize);
        void prepareColorData(int theNumberOfVertices);
        void colors(ShaderTexturePtr theShaderTexture);
        void colors(ShaderTexturePtr theShaderTexture, int theID);
        void colors(ShaderTexturePtr theShaderTexture, int theX, int theY, int theWidth, int theHeight);
        void colors(ShaderTexturePtr theShaderTexture, int theID, int theX, int theY, int theWidth, int theHeight);
        void enable();
        void disable();
};

};// namespace

#endif
