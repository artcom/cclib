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

import cc.creativecomputing.control.CCControl;
import cc.creativecomputing.graphics.CCColor;
import cc.creativecomputing.math.CCVector3f;

/**
 * @author christianriekoff
 *
 */
public class CCPointLight extends CCDeferredLight{
	
	//this is the position of the light
	private CCVector3f _myPosition;
	
	//how far does this light reach
	private float _myRadius;
	
	//control the brightness of the light
	private float _myIntensity = 1.0f;
	
	public CCPointLight(
		CCColor theColor, 
		CCVector3f theLightPosition,
		float theLightRadius,
		float theLightIntensity
	) {
		super(theColor);
		_myPosition = theLightPosition;
		_myRadius = theLightRadius;
		_myIntensity = theLightIntensity;
	}
	
	public CCPointLight() {
		this(new CCColor(255), new CCVector3f(), 10, 1.0f);
	}
	
	public CCVector3f position() {
		return _myPosition;
	}
	
	public void position(float theX, float theY, float theZ) {
		_myPosition.set(theX, theY, theZ);
	}
	
	public void position(CCVector3f theDirection) {
		_myPosition.set(theDirection);
	}
	
	@CCControl(name = "x", min = -1000, max = 1000)
	public void x(float theX) {
		_myPosition.x = theX;
	}
	
	@CCControl(name = "y", min = -1000, max = 1000)
	public void y(float theY) {
		_myPosition.y = theY;
	}
	
	@CCControl(name = "z", min = -1000, max = 1000)
	public void z(float theZ) {
		_myPosition.z = theZ;
	}
	
	@CCControl(name = "intensity", min = 0, max = 1)
	public void intensity(float theIntensity) {
		_myIntensity = theIntensity;
	}
	
	public float intensity() {
		return _myIntensity;
	}
	
	@CCControl(name = "radius", min = 0, max = 1000)
	public void radius(float theRadius) {
		_myRadius = theRadius;
	}
	
	public float radius() {
		return _myRadius;
	}
}
