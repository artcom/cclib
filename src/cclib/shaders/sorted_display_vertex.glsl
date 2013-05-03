#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform sampler2DRect positions;
uniform sampler2DRect indices;
uniform sampler2DRect infos;
uniform sampler2DRect colors;

uniform float pointSize;

void main (){
	vec4 myIndices = texture2DRect(indices, gl_Vertex.xy);
	vec4 myPosition = texture2DRect(positions, myIndices.xy);
	vec4 myRandoms =  texture2DRect(colors, myIndices.xy);
	vec4 myColor = texture2DRect(colors, myIndices.xy);// * gl_Color;
	//vec4 myColor = gl_Color;
	//myColor.xyz +=myRandoms.x * 0.5;
	gl_Position = gl_ModelViewProjectionMatrix * myPosition;
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);
	
	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * myPosition;
	gl_PointSize =  max(tanHalfFOV / -posViewSpace.z * pointSize, 1) * myRandoms.y;
	
	vec4 myValues = texture2DRect(infos, myIndices.xy);
	float myAlpha = clamp(1 - myValues.x / myValues.y,0,1);
	
	gl_FrontColor = min(myColor * gl_PointSize * gl_PointSize, myColor);
	gl_FrontColor.a *= myAlpha * myAlpha;
}
	           