/*  
 * Copyright (c) 2011 Christian Riekoff <info@texone.org>  
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
import cc.creativecomputing.graphics.CCGraphics;
import cc.creativecomputing.graphics.shader.CCGLSLShader;
import cc.creativecomputing.graphics.shader.CCShaderBuffer;
import cc.creativecomputing.graphics.texture.CCTexture2D;
import cc.creativecomputing.graphics.texture.CCTextureIO;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureFilter;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureTarget;
import cc.creativecomputing.graphics.texture.CCTexture.CCTextureWrap;
import cc.creativecomputing.io.CCIOUtil;

/**
 * @author christianriekoff
 *
 */
public class CCSSAO extends CCPostProcessEffect{
	@CCControl(name = "sample radius", min = 0, max = 20)
	private float sampleRadius = 0.4f;
	
	@CCControl(name = "intensity", min = 0, max = 20)
	float intensity = 2.5f;

	@CCControl(name = "scale", min = 0, max = 1f)
	float scale = 0.34f;

	@CCControl(name = "bias", min = 0, max = 1)
	float bias = 0.05f;

	@CCControl(name = "jitter", min = 0, max = 100)
	float jitter = 64.0f;

	@CCControl(name = "self occlusion", min = 0, max = 1)
	float selfOcclusion = 0.12f;
	
	private CCGLSLShader	mSSAOShader;
	private CCTexture2D		mSSAORandTexture;
	
	private CCShaderBuffer	mFbo;
	private CCShaderBuffer	mTempFbo;
	
	public CCSSAO() {
		// Ambient Occlusion
		 mSSAOShader = new CCGLSLShader(
			CCIOUtil.classPath(this, "postProcess_vert.glsl"),
			CCIOUtil.classPath(this, "ssao.glsl")
		 );
		 mSSAOShader.load();
		 
		 mSSAORandTexture = new CCTexture2D(CCTextureIO.newTextureData(CCIOUtil.classPath(this, "randomNormals.png")));
		 mSSAORandTexture.wrap(CCTextureWrap.REPEAT);
		 mSSAORandTexture.textureFilter(CCTextureFilter.NEAREST);
	}
	
	@Override
	public void initialize(int theWidth, int theHeight) {
		mFbo = new CCShaderBuffer(theWidth, theHeight, CCTextureTarget.TEXTURE_2D);
		mTempFbo = new CCShaderBuffer(theWidth, theHeight, CCTextureTarget.TEXTURE_2D);
	}
	
	@Override
	public void apply(CCGeometryBuffer theGeometryBuffer, CCGraphics g){

		g.texture(0, theGeometryBuffer.positions());
		g.texture(1, theGeometryBuffer.normals());
		g.texture(2, mSSAORandTexture);

		mSSAOShader.start();

		mSSAOShader.uniform1i( "positions", 0 );
		mSSAOShader.uniform1i( "normals", 1 );
		mSSAOShader.uniform1i( "random", 2 );

		mSSAOShader.uniform1f( "sampleRadius", sampleRadius );
		mSSAOShader.uniform1f( "intensity", intensity );
		mSSAOShader.uniform1f( "scale", scale /25f);
		mSSAOShader.uniform1f( "bias", bias );
		mSSAOShader.uniform1f( "jitter", jitter );
		mSSAOShader.uniform1f( "selfOcclusion", selfOcclusion );
		mSSAOShader.uniform2f( "screenSize", mFbo.width(), mFbo.height());
		mSSAOShader.uniform2f( "invScreenSize", 1.0f /  mFbo.width(), 1.0f / mFbo.height());

		g.color(255);
		mFbo.draw();
////		GL2 gl = CCGraphics.currentGL();
////		gl.glBegin(GL2.GL_QUADS);
////		gl.glTexCoord2f(0, 0);
////		gl.glVertex2f(0, 0);
////		gl.glTexCoord2f(1f, 0);
////		gl.glVertex2f(mFbo.width(), 0);
////		gl.glTexCoord2f(1f, 1f);
////		gl.glVertex2f(mFbo.width(), mFbo.height());
////		gl.glTexCoord2f(0, 1f);
////		gl.glVertex2f(0, mFbo.height());
////		gl.glEnd();

		mSSAOShader.end();

		g.noTexture();
	}

	public CCTexture2D content() {
		return mFbo.attachment(0);
	}
}
