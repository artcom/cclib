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
public class CCDirectionalLight extends CCDeferredLight{
	//direction of the light
	private CCVector3f _myLightDirection;
	
	public CCDirectionalLight(CCColor theColor, CCVector3f theDirection) {
		super(theColor);
		_myLightDirection = theDirection;
	}
	
	public CCDirectionalLight() {
		this(new CCColor(), new CCVector3f());
	}
	
	public CCVector3f lightDirection() {
		return _myLightDirection;
	}
	
	public void lightDirection(float theX, float theY, float theZ) {
		_myLightDirection.set(theX, theY, theZ);
	}
	
	public void lightDirection(CCVector3f theDirection) {
		_myLightDirection.set(theDirection);
	}
	
	@CCControl(name = "x", min = -1, max = 1)
	public void x(float theX) {
		_myLightDirection.x = theX;
	}
	
	@CCControl(name = "y", min = -1, max = 1)
	public void y(float theY) {
		_myLightDirection.y = theY;
	}
	
	@CCControl(name = "z", min = -1, max = 1)
	public void z(float theZ) {
		_myLightDirection.z = theZ;
	}
}
