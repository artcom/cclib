
#ifndef __CCLIB_VBOMESH_INCLUDED__
#define __CCLIB_VBOMESH_INCLUDED__

#include <cclib.h>

namespace cclib {

class VBOMesh {
    
    private:
        CCBufferObject _myVertexBuffer;	
        bool _myHasUpdatedVertices = true;
        bool _myHasVertices = false;

        CCBufferObject[] _myTextureCoordBuffers = new CCBufferObject[8];
        bool[] _myHasUpdatedTextureCoords = new bool[] {true, true, true, true, true, true, true, true};
        bool[] _myHasTextureCoords = new bool[] {false, false, false, false, false, false, false, false};

        CCBufferObject _myColorBuffer;
        bool _myHasUpdatedColors = true;
        bool _myHasColors = false;

        CCBufferObject _myNormalBuffer;
        bool _myHasUpdatedNormals = true;
        bool _myHasNormals = false;

        bool _myHasIndices = true;

        bool _myHasDefinedSize = false;

};

public class CCVBOMesh extends CCMesh {

    
    /**
     * Create a new VBO Mesh, where draw mode is set to QUADS
     */
	public CCVBOMesh(){
    	super(CCDrawMode.QUADS);
	}
	
	public CCVBOMesh(final CCDrawMode theDrawMode){
    	super(theDrawMode);
    }
	
	public CCVBOMesh(final CCDrawMode theDrawMode, final int theNumberOfVertices){
		this(theDrawMode, theNumberOfVertices,3);
	}
	
	public CCVBOMesh(final CCDrawMode theDrawMode, final int theNumberOfVertices, final int theNumberOfVertexCoords){
		super(theDrawMode, theNumberOfVertices);
		
		_myHasDefinedSize = true;
		_myVertexSize = theNumberOfVertexCoords;
		_myHasVertices = true;
	}
    
    public CCVBOMesh(
    	final List<CCVector3f> theVertices,
    	final List<CCVector2f> theTextureCoords,
    	final List<CCColor> theColors
    ){
    	super(theVertices, theTextureCoords, theColors);
    	_myHasDefinedSize = true;
    }
    
    public CCVBOMesh(
    	final CCDrawMode theDrawMode,
        final List<CCVector3f> theVertices,
        final List<CCVector2f> theTextureCoords,
        final List<CCColor> theColors
    ){
    	super(theDrawMode, theVertices, theTextureCoords, theColors);
    	_myHasDefinedSize = true;
    }
    
    private void bufferSubData(final int[] theBufferID, final FloatBuffer theData, final int theOffset, final int theNumberOfVertices, final int theNumberOfCoords) {
    	theData.flip();
    	GL gl = CCGraphics.currentGL();
        
        // Bind The Buffer
        gl.glBindBuffer(GL.GL_ARRAY_BUFFER, theBufferID[0]);
        
        // Load The Data
        gl.glBufferSubData(GL.GL_ARRAY_BUFFER, theOffset, theNumberOfVertices * theNumberOfCoords * CCBufferUtil.SIZE_OF_FLOAT, theData);
    
    }
    
    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD VERTEX DATA
    //
    //////////////////////////////////////////////////////
    
    @Override
    public void prepareVertexData(int theNumberOfVertices, int theVertexSize){
    	_myNumberOfVertices = theNumberOfVertices;
    	_myVertexSize = theVertexSize;

    	if(_myVertices == null || _myVertices.limit() / _myVertexSize != _myNumberOfVertices) {
    		_myVertexSize = theVertexSize;
    		if(_myVertexBuffer == null) {
    			_myVertexBuffer = new CCBufferObject(_myNumberOfVertices * _myVertexSize * CCBufferUtil.SIZE_OF_FLOAT);
    		}else {
    			_myVertexBuffer.bufferData(_myNumberOfVertices * _myVertexSize * CCBufferUtil.SIZE_OF_FLOAT, null, CCUsageFrequency.DYNAMIC, CCUsageTYPE.DRAW);
    		}
    		_myVertices = _myVertexBuffer.data().asFloatBuffer();
    		_myVertices.rewind();
    	}
		_myHasUpdatedVertices = true;
		_myHasVertices = true;
    }
    
    public void vertices(CCShaderTexture theShaderTexture){
    	vertices(theShaderTexture,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void vertices(CCShaderTexture theShaderTexture, final int theID){
    	vertices(theShaderTexture,0,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void vertices(CCShaderTexture theShaderTexture, final int theX, final int theY, final int theWidth, final int theHeight) {
    	vertices(theShaderTexture, 0, theX, theY, theWidth, theHeight);
    }
    
    public void vertices(CCShaderTexture theShaderTexture, final int theID, final int theX, final int theY, final int theWidth, final int theHeight) {
    	if(_myVertexBuffer == null)_myVertexBuffer = new CCBufferObject();
    	
    	_myVertexSize = theShaderTexture.numberOfChannels();
    	_myNumberOfVertices = theWidth * theHeight;
    	
		_myVertexBuffer.copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);
    	
    	_myHasVertices = true;
		_myHasUpdatedVertices = false;
    }
	
	public CCBufferObject vertexBuffer() {
		prepareVertexData(_myVertexSize);
		return _myVertexBuffer;
	}
    
    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD NORMAL DATA
    //
    //////////////////////////////////////////////////////
	
	@Override
	public void prepareNormalData(int theNumberOfVertices){
		_myNumberOfVertices = theNumberOfVertices;
		
    	if(_myNormalBuffer == null || _myNormals.limit() / 3 != _myNumberOfVertices) {
    		_myNumberOfVertices = theNumberOfVertices;
    		_myNormalBuffer = new CCBufferObject(_myNumberOfVertices * 3 * CCBufferUtil.SIZE_OF_FLOAT);
    		_myNormals = _myNormalBuffer.data().asFloatBuffer();
    		_myNormals.rewind();
    	}
		_myHasNormals = true;
    	_myHasUpdatedNormals = true;
    }
    
    public void normals(CCShaderTexture theShaderTexture){
    	normals(theShaderTexture,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void normals(CCShaderTexture theShaderTexture, final int theID){
    	normals(theShaderTexture,theID,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void normals(CCShaderTexture theShaderTexture, final int theX, final int theY, final int theWidth, final int theHeight) {
    	normals(theShaderTexture, 0, theX, theY, theWidth, theHeight);
    }
    
    public void normals(CCShaderTexture theShaderTexture, final int theID, final int theX, final int theY, final int theWidth, final int theHeight) {    	
    	if(_myNormalBuffer == null)_myNormalBuffer = new CCBufferObject();
		
		_myNormalBuffer.copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);
    	
    	_myHasNormals = true;
		_myHasUpdatedNormals = false;
    }
    
    //////////////////////////////////////////////////////
    //
    //  METHODS TO ADD TEXTURE COORD DATA
    //
    //////////////////////////////////////////////////////
	
    @Override
	 public void prepareTextureCoordData(int theNumberOfVertices, int theLevel, int theTextureCoordSize){
    	_myNumberOfVertices = theNumberOfVertices;
    	_myTextureCoordSize[theLevel] = theTextureCoordSize;
    	if(_myTextureCoordBuffers[theLevel] == null || _myTextureCoords[theLevel].limit() / _myTextureCoordSize[theLevel] != _myNumberOfVertices) {
    		if(_myTextureCoordBuffers[theLevel] == null) {
    			_myTextureCoordBuffers[theLevel] = new CCBufferObject(_myNumberOfVertices * theTextureCoordSize * CCBufferUtil.SIZE_OF_FLOAT);
    		}else {
    			_myTextureCoordBuffers[theLevel].bufferData(_myNumberOfVertices * theTextureCoordSize * CCBufferUtil.SIZE_OF_FLOAT, null, CCUsageFrequency.DYNAMIC, CCUsageTYPE.DRAW);
    		}

    		_myTextureCoords[theLevel] = _myTextureCoordBuffers[theLevel].data().asFloatBuffer();
	    	_myTextureCoords[theLevel].rewind();
	    }
    	
    	_myHasTextureCoords[theLevel] = true;
    	_myHasUpdatedTextureCoords[theLevel] = true;
    }
	
	//////////////////////////////////////////////////
	//
	// METHODS TO ADD COLOR DATA
	//
	//////////////////////////////////////////////////
	
	@Override
	public void prepareColorData(int theNumberOfVertices){
		_myNumberOfVertices = theNumberOfVertices;
		
    	if(_myColorBuffer == null || _myColors.limit() / 4 != _myNumberOfVertices){
    		_myColorBuffer = new CCBufferObject(_myNumberOfVertices * 4 * CCBufferUtil.SIZE_OF_FLOAT);
    		_myColors = _myColorBuffer.data().asFloatBuffer();
    		_myColors.rewind();
    	}
    	
    	_myHasColors = true;
		_myHasUpdatedColors = true;
    }
    
    public void colors(CCShaderTexture theShaderTexture){
    	colors(theShaderTexture,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void colors(CCShaderTexture theShaderTexture, final int theID){
    	colors(theShaderTexture,theID,0,0,theShaderTexture.width(), theShaderTexture.height());
    }
    
    public void colors(CCShaderTexture theShaderTexture, final int theX, final int theY, final int theWidth, final int theHeight) {
    	colors(theShaderTexture, 0, theX, theY, theWidth, theHeight);
    }
    
    public void colors(CCShaderTexture theShaderTexture, final int theID, final int theX, final int theY, final int theWidth, final int theHeight) {
    	if(_myColorBuffer == null){
    		_myColorBuffer = new CCBufferObject(theWidth * theHeight * CCBufferUtil.SIZE_OF_FLOAT);
    	}
    	
    	_myColorBuffer.copyDataFromTexture(theShaderTexture, theID, theX, theY, theWidth, theHeight);
    	
    	_myHasColors = true;
    	_myHasUpdatedColors = false;
    }
	
	@Override
	public void enable(final CCGraphics g){
		// Enable Pointers
		for(int i = 0; i < _myHasTextureCoords.length;i++) {
	    	if(_myHasTextureCoords[i]){
	    		if(_myHasUpdatedTextureCoords[i]) {
	    			_myTextureCoordBuffers[i].bind(CCBufferTarget.ARRAY);
	    			_myTextureCoordBuffers[i].bufferData();
	    			_myTextureCoordBuffers[i].unbind();
	        		_myHasUpdatedTextureCoords[i] = false;
	    		}
	    		_myTextureCoordBuffers[i].bind(CCBufferTarget.ARRAY);
	    		g.gl.glClientActiveTexture(GL.GL_TEXTURE0 + i);
	    		g.gl.glEnableClientState(GL2.GL_TEXTURE_COORD_ARRAY);
	    		g.gl.glTexCoordPointer(_myTextureCoordSize[i], GL.GL_FLOAT, 0, 0);
	    	}
		}
    	if(_myHasColors){
    		if(_myHasUpdatedColors) {
    			_myColorBuffer.bind(CCBufferTarget.ARRAY);
    			_myColorBuffer.bufferData();
    			_myColorBuffer.unbind();
        		_myHasUpdatedColors = false;
    		}
    		_myColorBuffer.bind(CCBufferTarget.ARRAY);
    		g.gl.glEnableClientState(GL2.GL_COLOR_ARRAY);
    		g.gl.glColorPointer(4, GL.GL_FLOAT, 0, 0);
    	}
    	if(_myHasNormals){
    		if(_myHasUpdatedNormals) {
    			_myNormalBuffer.bind(CCBufferTarget.ARRAY);
    			_myNormalBuffer.bufferData();
    			_myNormalBuffer.unbind();
    			_myHasUpdatedNormals = false;
    		}
    		_myNormalBuffer.bind(CCBufferTarget.ARRAY);
    		g.gl.glEnableClientState(GL2.GL_NORMAL_ARRAY);
    		g.gl.glNormalPointer(GL.GL_FLOAT, 0, 0);
    	}
    	if(_myHasVertices){
    		if(_myHasUpdatedVertices) {
    			_myVertexBuffer.bind(CCBufferTarget.ARRAY);
    			_myVertexBuffer.bufferData();
    			_myVertexBuffer.unbind();
    			_myHasUpdatedVertices = false;
    		}
    		_myVertexBuffer.bind(CCBufferTarget.ARRAY);
    		g.gl.glEnableClientState(GL2.GL_VERTEX_ARRAY);
 	   		g.gl.glVertexPointer(_myVertexSize, GL.GL_FLOAT, 0, 0);
    	}
    	if(_myHasIndices) {
    		
    	}
    	
//    	if(_myDrawMode == CCGraphics.POINTS && g._myDrawTexture){
//			g.gl.glEnable(GL.GL_POINT_SPRITE);
//			g.gl.glTexEnvi(GL.GL_POINT_SPRITE, GL.GL_COORD_REPLACE, GL.GL_TRUE); 
//		}
	}
	
	@Override
	public void disable(CCGraphics g){
		super.disable(g);
        
        g.gl.glBindBuffer(GL.GL_ARRAY_BUFFER, 0);
	}
};

};// namespace

#endif
