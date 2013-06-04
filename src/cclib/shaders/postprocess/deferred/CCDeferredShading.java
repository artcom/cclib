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
package cc.creativecomputing.graphics.shader.postprocess.deferred;

import java.util.ArrayList;
import java.util.List;

import cc.creativecomputing.graphics.CCAbstractGraphics.CCBlendMode;
import cc.creativecomputing.graphics.CCDrawMode;
import cc.creativecomputing.graphics.CCGraphics;
import cc.creativecomputing.graphics.CCRenderBuffer;
import cc.creativecomputing.graphics.shader.CCGLSLShader;
import cc.creativecomputing.graphics.shader.postprocess.CCGeometryBuffer;
import cc.creativecomputing.graphics.shader.postprocess.CCPostProcessEffect;
import cc.creativecomputing.graphics.texture.CCTexture2D;
import cc.creativecomputing.io.CCIOUtil;
import cc.creativecomputing.math.CCMath;
import cc.creativecomputing.math.CCVecMath;
import cc.creativecomputing.math.CCVector3f;

/**
 * @author christianriekoff
 *
 */
public class CCDeferredShading extends CCPostProcessEffect{
	
	private CCRenderBuffer	_myRenderBuffer;
	
	private CCGLSLShader _myDirectionalLightShader;
	private CCGLSLShader _myPointLightShader;
	private CCGLSLShader _mySpotLightShader;
	
	private List<CCDirectionalLight> _myDirectionalLights = new ArrayList<CCDirectionalLight>();
	private List<CCPointLight> _myPointLights = new ArrayList<CCPointLight>();
	private List<CCSpotLight> _mySpotLights = new ArrayList<CCSpotLight>();
	
	private int _myWidth;
	private int _myHeight;
	
	private CCGraphics _myGraphics;
	
	public CCDeferredShading(CCGraphics g) {
		_myGraphics = g;
		_myDirectionalLightShader = new CCGLSLShader(
			CCIOUtil.classPath(this, "directionalLight_vert.glsl"),
			CCIOUtil.classPath(this, "directionalLight.glsl")
		);
		_myDirectionalLightShader.load();
		
		_myPointLightShader = new CCGLSLShader(
			CCIOUtil.classPath(this, "pointLight_vert.glsl"),
			CCIOUtil.classPath(this, "pointLight.glsl")
		);
		_myPointLightShader.load();
		
		_mySpotLightShader = new CCGLSLShader(
			CCIOUtil.classPath(this, "spotLight_vert.glsl"),
			CCIOUtil.classPath(this, "spotLight.glsl")
		);
		_mySpotLightShader.load();
	}
	
	public void add(CCDirectionalLight theDirectionalLight) {
		_myDirectionalLights.add(theDirectionalLight);
	}
	
	public void add(CCPointLight thePointLight) {
		_myPointLights.add(thePointLight);
	}
	
	public void add(CCSpotLight theSpotLight){
		_mySpotLights.add(theSpotLight);
	}
	
	@Override
	public void initialize(int theWidth, int theHeight) {
		_myRenderBuffer = new CCRenderBuffer(_myGraphics, theWidth, theHeight);
		_myWidth = theWidth;
		_myHeight = theHeight;
	}
	
	private void applyDirectionalLights() {
		_myDirectionalLightShader.start();
		_myDirectionalLightShader.uniform3f("inCamPosition", _myGraphics.camera().position());
		_myDirectionalLightShader.uniform1i("positions", 0);
		_myDirectionalLightShader.uniform1i("normals", 1);
		_myDirectionalLightShader.uniform2f("screenSize", _myWidth, _myHeight);
		
		for(CCDirectionalLight myDirectionalLight:_myDirectionalLights) {
			CCVector3f myDirection = myDirectionalLight.lightDirection().clone().normalize().negate();
			_myDirectionalLightShader.uniform3f("lightDirection", myDirection);
			_myDirectionalLightShader.uniform3f(
				"lightColor", 
				myDirectionalLight.color().r, 
				myDirectionalLight.color().g, 
				myDirectionalLight.color().b
			);
			_myDirectionalLightShader.uniform1f("specularPower", myDirectionalLight.specularPower());
			_myDirectionalLightShader.uniform1f("specularIntensity", myDirectionalLight.specularIntensity());
			
			_myGraphics.beginShape(CCDrawMode.QUADS);
			_myGraphics.vertex(-_myWidth/2, -_myHeight/2);
			_myGraphics.vertex( _myWidth/2, -_myHeight/2);
			_myGraphics.vertex( _myWidth/2,  _myHeight/2);
			_myGraphics.vertex(-_myWidth/2,  _myHeight/2);
			_myGraphics.endShape();
		}
		
		_myDirectionalLightShader.end();
	}
	
	private void applyPointLights() {
		_myPointLightShader.start();
		_myPointLightShader.uniform3f("inCamPosition", _myGraphics.camera().position());
		_myPointLightShader.uniform1i("positions", 0);
		_myPointLightShader.uniform1i("normals", 1);
		_myPointLightShader.uniform2f("screenSize", _myWidth, _myHeight);
		
		for(CCPointLight myPointLight:_myPointLights) {
			CCVector3f myPosition = myPointLight.position();
			_myPointLightShader.uniform3f("position", myPosition);
			_myPointLightShader.uniform3f(
				"lightColor", 
				myPointLight.color().r, 
				myPointLight.color().g, 
				myPointLight.color().b
			);
			_myPointLightShader.uniform1f("intensity", myPointLight.intensity());
			_myPointLightShader.uniform1f("radius", myPointLight.radius() / 2);

			_myPointLightShader.uniform1f("specularPower", myPointLight.specularPower());
			_myPointLightShader.uniform1f("specularIntensity", myPointLight.specularIntensity());
			
			float myLightX = myPointLight.position().x;
			float myLightY = myPointLight.position().y;
			float myLightZ = myPointLight.position().z;

			_myGraphics.beginShape(CCDrawMode.QUADS);
			_myGraphics.vertex(myLightX - myPointLight.radius(), myLightY - myPointLight.radius(), myLightZ);
			_myGraphics.vertex(myLightX + myPointLight.radius(), myLightY - myPointLight.radius(), myLightZ);
			_myGraphics.vertex(myLightX + myPointLight.radius(), myLightY + myPointLight.radius(), myLightZ);
			_myGraphics.vertex(myLightX - myPointLight.radius(), myLightY + myPointLight.radius(), myLightZ);
			_myGraphics.endShape();
		}
		
		_myPointLightShader.end();
	}

	private void applySpotLights() {
		_mySpotLightShader.start();
		_mySpotLightShader.uniform1i("positions", 0);
		_mySpotLightShader.uniform1i("normals", 1);
		_mySpotLightShader.uniform2f("screenSize", _myWidth, _myHeight);
		
		for(CCSpotLight mySpotLight:_mySpotLights) {
			CCVector3f myPosition = mySpotLight.position();
			_mySpotLightShader.uniform3f("position", myPosition);
			_mySpotLightShader.uniform3f("direction", mySpotLight.direction());
			_mySpotLightShader.uniform3f(
				"lightColor", 
				mySpotLight.color().r, 
				mySpotLight.color().g, 
				mySpotLight.color().b
			);
			_mySpotLightShader.uniform1f("intensity", mySpotLight.intensity());
			_mySpotLightShader.uniform1f("radius", mySpotLight.radius());
			_mySpotLightShader.uniform1f("spotDecayExponent", mySpotLight.spotDecayExponent());
			_mySpotLightShader.uniform1f("spotLightAngleCosine", CCMath.cos(CCMath.radians(mySpotLight.coneAngle())));
			float myHeight = CCMath.sin(CCMath.radians(mySpotLight.coneAngle())) * mySpotLight.radius();
			
			CCVector3f myFirst = mySpotLight.direction().perp().normalize(myHeight);
			CCVector3f mySecond = CCVecMath.cross(myFirst, mySpotLight.direction()).normalize(myHeight);
			CCVector3f myEnd = CCVecMath.add(mySpotLight.position(), mySpotLight.direction().clone().normalize(mySpotLight.radius()));
			
			CCVector3f myA = CCVecMath.add(myEnd, myFirst, mySecond);
			CCVector3f myB = CCVecMath.add(myEnd, myFirst.negate(), mySecond);
			CCVector3f myC = CCVecMath.add(myEnd, myFirst, mySecond.negate());
			CCVector3f myD = CCVecMath.add(myEnd, myFirst.negate(), mySecond);

			_myGraphics.beginShape(CCDrawMode.TRIANGLES);
			_myGraphics.vertex(myPosition);
			_myGraphics.vertex(myA);
			_myGraphics.vertex(myB);
			_myGraphics.vertex(myPosition);
			_myGraphics.vertex(myB);
			_myGraphics.vertex(myC);
			_myGraphics.vertex(myPosition);
			_myGraphics.vertex(myC);
			_myGraphics.vertex(myD);
			_myGraphics.vertex(myPosition);
			_myGraphics.vertex(myD);
			_myGraphics.vertex(myA);
			_myGraphics.endShape();

			_myGraphics.beginShape(CCDrawMode.QUADS);
			_myGraphics.vertex(myA);
			_myGraphics.vertex(myB);
			_myGraphics.vertex(myC);
			_myGraphics.vertex(myD);
			_myGraphics.endShape();
		}
		
		_mySpotLightShader.end();
	}
	
	@Override
	public void apply(CCGeometryBuffer theGeometryBuffer, CCGraphics g) {
		g.pushAttribute();
		g.blend(CCBlendMode.ADD);
		g.noDepthTest();
		
		g.color(255);
		_myRenderBuffer.beginDraw();
		g.clear();
		
		g.texture(0, theGeometryBuffer.positions());
		g.texture(1, theGeometryBuffer.normals());
		g.texture(2, theGeometryBuffer.colors());

		applyDirectionalLights();
		applyPointLights();
		applySpotLights();

		g.noTexture();
		
		g.noBlend();
//		g.color(255,0,0);
//		g.polygonMode(CCPolygonMode.LINE);
//		for(CCPointLight myPointLight:_myPointLights) {
//			CCVector3f myPosition = myPointLight.position();
//			
//			float myLightX = myPointLight.position().x;
//			float myLightY = myPointLight.position().y;
//			float myLightZ = myPointLight.position().z;
//
//			_myGraphics.beginShape(CCDrawMode.QUADS);
//			_myGraphics.vertex(myLightX - myPointLight.radius(), myLightY - myPointLight.radius(), myLightZ);
//			_myGraphics.vertex(myLightX + myPointLight.radius(), myLightY - myPointLight.radius(), myLightZ);
//			_myGraphics.vertex(myLightX + myPointLight.radius(), myLightY + myPointLight.radius(), myLightZ);
//			_myGraphics.vertex(myLightX - myPointLight.radius(), myLightY + myPointLight.radius(), myLightZ);
//			_myGraphics.endShape();
//		}
//		g.polygonMode(CCPolygonMode.FILL);
		_myRenderBuffer.endDraw();
		
		g.popAttribute();
		
	}

	public CCTexture2D content() {
		return _myRenderBuffer.attachment(0);
	}
}
