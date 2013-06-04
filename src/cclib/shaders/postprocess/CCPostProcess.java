package cc.creativecomputing.graphics.shader.postprocess;

import java.util.ArrayList;
import java.util.List;

import javax.media.opengl.GL2;

import cc.creativecomputing.control.CCControl;
import cc.creativecomputing.graphics.CCGraphics;
import cc.creativecomputing.graphics.CCRenderBuffer;
import cc.creativecomputing.graphics.shader.CCGLSLShader;
import cc.creativecomputing.graphics.shader.CCShaderBuffer;
import cc.creativecomputing.graphics.texture.CCFrameBufferObjectAttributes;
import cc.creativecomputing.graphics.texture.CCTexture2D;
import cc.creativecomputing.graphics.texture.CCTextureIO;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureFilter;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureTarget;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureWrap;
import cc.creativecomputing.io.CCIOUtil;

public class CCPostProcess {

	private CCGeometryBuffer _myGeometryBuffer;

	private CCGraphics _myGraphics;
	
	private List<CCPostProcessEffect> _myEffects = new ArrayList<CCPostProcessEffect>();

	public CCPostProcess(CCGraphics g, int width, int height) {
		_myGeometryBuffer = new CCGeometryBuffer(g, width, height);
		_myGraphics = g;
	}
	
	public void addEffect(CCPostProcessEffect theEffect) {
		theEffect.initialize(_myGeometryBuffer.data().width(), _myGeometryBuffer.data().height());
		_myEffects.add(theEffect);
	}

	public void beginDraw() {
		_myGeometryBuffer.beginDraw();
		_myGraphics.noBlend();
	}

	public void endDraw() {
		_myGeometryBuffer.endDraw();
		
		for(CCPostProcessEffect myEffect:_myEffects) {
			myEffect.apply(_myGeometryBuffer, _myGraphics);
		}
	}

	public CCGeometryBuffer geometryBuffer() {
		return _myGeometryBuffer;
	}
};