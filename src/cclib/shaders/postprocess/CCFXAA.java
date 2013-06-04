/*  
 * Copyright (c) 2012 Christian Riekoff <info@texone.org>  
 *  
 *  This file is free software: you may copy, redistribute and/or modify it  
 *  under the terms of the GNU General Public License as published by the  
 *  Free Software Foundation, either version 2 of the License, or (at your  
 *  option) any later version.  
 *  
 *  This file is distributed in the hope that it will be useful, but  
 *  WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  
 *  General Public License for more details.  
 *  
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 *  
 * This file incorporates work covered by the following copyright and  
 * permission notice:  
 */
package cc.creativecomputing.graphics.shader.postprocess;

import cc.creativecomputing.control.CCControl;
import cc.creativecomputing.graphics.CCDrawMode;
import cc.creativecomputing.graphics.CCGraphics;
import cc.creativecomputing.graphics.shader.CCGLSLShader;
import cc.creativecomputing.graphics.texture.CCTexture2D;
import cc.creativecomputing.io.CCIOUtil;

/**
 * @author christianriekoff
 *
 */
public class CCFXAA {

	private CCGLSLShader _myFXAAShader;
	private CCTexture2D _myContentTexture;
	
	@CCControl(name = "subpixel shift", min = 0, max = 1)
	private float _cSubPixelShift = 0.25f;
	
	@CCControl(name = "span max", min = 0, max = 20)
	private float _cSpanMax = 8f;
	
	public CCFXAA(CCTexture2D theContentTexture) {
		_myFXAAShader = new CCGLSLShader(
			CCIOUtil.classPath(CCPostProcess.class, "shader/fxaa_vertex.glsl"), 
			CCIOUtil.classPath(CCPostProcess.class, "shader/fxaa_fragment.glsl")
		);
		_myFXAAShader.load();
		_myContentTexture = theContentTexture;
	}
	
	public void draw(CCGraphics g) {
		_myFXAAShader.start();
		_myFXAAShader.uniform1i("tex0", 0);
		_myFXAAShader.uniform2f("invTextureSize", 1f / _myContentTexture.width(), 1f / _myContentTexture.height());
		_myFXAAShader.uniform1f("subpixelShift", _cSubPixelShift);
		_myFXAAShader.uniform1f("FXAA_SPAN_MAX", _cSpanMax);
		_myFXAAShader.uniform1f("FXAA_REDUCE_MUL", 1.0f/_cSpanMax);
		g.texture(0, _myContentTexture);
		g.beginShape(CCDrawMode.QUADS);
		g.textureCoords(0, 0f, 0f);
		g.vertex(-_myContentTexture.width() / 2,   _myContentTexture.height() / 2);
		g.textureCoords(0, 1f, 0f);
		g.vertex( _myContentTexture.width() / 2,   _myContentTexture.height() / 2);
		g.textureCoords(0, 1f, 1f);
		g.vertex( _myContentTexture.width() / 2,  -_myContentTexture.height() / 2);
		g.textureCoords(0, 0f, 1f);
		g.vertex(-_myContentTexture.width() / 2,  -_myContentTexture.height() / 2);
		g.endShape();
		g.noTexture();
//		g.image(_myRenderTexture, -width/2, -height/2);
		_myFXAAShader.end();
	}
}
