#ifndef __CCLIB_GPUFORCE_INCLUDED__
#define __CCLIB_GPUFORCE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec2.h>
#include <math/vec3.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <Exception.h>

namespace cclib {

class GPUForce {

    protected:
        std::string _myParameterIndex;
        std::string _myShaderTypeName;
        GPUUpdateShader::Ptr _myVelocityShader;
        GPUParticles::Ptr _myParticles;
	
	    float _cStrength = 1;
	
        CGparameter _myStrengthParameter;
	
        void setupParameter(int theWidth, int theHeight) {
            _myStrengthParameter = parameter("strength");
        }
// - -- --- 	
	protected CCGPUForce(final String theShaderTypeName){
		_myShaderTypeName = theShaderTypeName;
	}
	
	public void setShader(CCGPUParticles theParticles, CCGPUUpdateShader theShader, final int theIndex, final int theWidth, final int theHeight){
		setShader(theParticles,theShader,"forces["+theIndex+"]", theWidth, theHeight);
	}
	
	public void setShader(CCGPUParticles theParticles, CCGPUUpdateShader theShader, final String theIndex, final int theWidth, final int theHeight) {
		_myParticles = theParticles;
		_myVelocityShader = theShader;
		_myParameterIndex = theIndex;
		_myVelocityShader.checkError("Problem creating force.");
		CgGL.cgConnectParameter(
			_myVelocityShader.createFragmentParameter(_myShaderTypeName), 
			_myVelocityShader.fragmentParameter(_myParameterIndex)
		);
		
        setupParameter(theWidth, theHeight);
//		update(0);
		_myVelocityShader.checkError("Problem creating force.");
	}
	
	/**
	 * @param theG
	 * @param theWidth
	 * @param theHeight
	 */
	public void setSize(CCGraphics theG, int theWidth, int theHeight) {
	}
	
	public void update(final float theDeltaTime) {
		_myVelocityShader.parameter(_myStrengthParameter, _cStrength);
	}
	
	public void reset() {
		
	}
	
	/**
	 * Set the strength of the force. The default value is one.
	 * @param theStrength strength value to scale the force
	 */
	public void strength(final float theStrength) {
		_cStrength = theStrength;
	}
	
	protected CGparameter parameter(final String theName){
		return _myVelocityShader.fragmentParameter(_myParameterIndex+"."+theName);
	}
}

