#version 120 
#extension GL_ARB_texture_rectangle : enable

void main() {
	
	gl_FragData[0] = gl_Color;
	gl_FragData[1] = gl_Color; //vec4(1.0, 0.0, 0.0, 1.0);
	//gl_FragData[2] = vec4(0.0, 0.0, 0.0, 0.0); 
	
    float linearDepth = gl_TexCoord[1].x;
    gl_FragData[2] = vec4(linearDepth, linearDepth, linearDepth, 1.0); // blur control
}
