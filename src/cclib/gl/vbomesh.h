
#ifndef __CCLIB_VBOMESH_INCLUDED__
#define __CCLIB_VBOMESH_INCLUDED__

#include <cclib.h>
#include <gl/bufferobject.h>
#include <gl/mesh.h>
#include <vector>
namespace cclib {

class VBOMesh : public Mesh {
    
    private:
        BufferObjectPtr _myVertexBuffer;	
        bool _myHasUpdatedVertices;
        bool _myHasVertices;

        //BufferObjectPtr _myTextureCoordBuffers[8]; // = new CCBufferObjectPtr[8];
        std::vector<BufferObjectPtr> _myTextureCoordBuffers; // = new CCBufferObjectPtr[8];
        bool _myHasUpdatedTextureCoords[8]; // = new bool[] {true, true, true, true, true, true, true, true};
        bool _myHasTextureCoords[8]; // = new bool[] {false, false, false, false, false, false, false, false};

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
        static VBOMeshPtr create(GLenum theDrawMode=GL_QUADS, int theNumberOfVertices=0);

        void bufferSubData(GLuint theBufferID, BufferPtr theData, int theOffset, 
                int theNumberOfVertices, int theNumberOfCoords); 
        void prepareVertexData(int theNumberOfVertices, int theVertexSize);
        void vertices(ShaderBufferPtr theShaderBuffer);
        void vertices(ShaderBufferPtr theShaderBuffer, GLuint theID);
        void vertices(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight);
        void vertices(ShaderBufferPtr theShaderBuffer, GLuint theID, int theX, int theY, int theWidth, int theHeight);
        BufferObjectPtr vertexBuffer();
        void prepareNormalData(int theNumberOfVertices);
        void normals(ShaderBufferPtr theShaderBuffer);
        void normals(ShaderBufferPtr theShaderBuffer, int theID);
        void normals(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight);
        void normals(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight);    	
        void prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize);
        void prepareColorData(int theNumberOfVertices);
        void colors(ShaderBufferPtr theShaderBuffer);
        void colors(ShaderBufferPtr theShaderBuffer, int theID);
        void colors(ShaderBufferPtr theShaderBuffer, int theX, int theY, int theWidth, int theHeight);
        void colors(ShaderBufferPtr theShaderBuffer, int theID, int theX, int theY, int theWidth, int theHeight);
        void enable();
        void disable();
};

};// namespace

#endif
