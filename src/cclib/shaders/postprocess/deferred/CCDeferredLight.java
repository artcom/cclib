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

/**
 * @author christianriekoff
 *
 */
public abstract class CCDeferredLight {

	//color of the light 
	protected CCColor _myColor;
	
	protected float _mySpecularIntensity = 0.0f;
	
	protected float _mySpecularPower = 255.0f;
	
	public CCDeferredLight(CCColor theColor) {
		_myColor = theColor;
	}
	
	public CCColor color() {
		return _myColor;
	}
	
	@CCControl(name = "red", min = 0, max = 1)
	public void red(float theRed) {
		_myColor.r = theRed;
	}
	
	@CCControl(name = "green", min = 0, max = 1)
	public void green(float theGreen) {
		_myColor.g = theGreen;
	}
	
	@CCControl(name = "blue", min = 0, max = 1)
	public void blue(float theBlue) {
		_myColor.b = theBlue;
	}
	
	@CCControl(name = "specular intensity", min = 0, max = 1)
	public void specularIntensity(float theSpecularIntensity){
		_mySpecularIntensity = theSpecularIntensity;
	}
	
	public float specularIntensity(){
		return _mySpecularIntensity;
	}
	
	@CCControl(name = "specular power", min = 0, max = 255)
	public void specularPower(float theSpecularPower){
		_mySpecularPower = theSpecularPower;
	}
	
	public float specularPower(){
		return _mySpecularPower;
	}
}
