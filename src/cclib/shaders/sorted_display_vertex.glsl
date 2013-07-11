
#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect positions;
uniform sampler2DRect infos;
uniform sampler2DRect indices;
uniform sampler2DRect colors;

uniform float tanHalfFOV;
uniform float pointSize;

void main (){
	vec4 myIndices = texture2DRect(indices, gl_Vertex.xy);
	vec4 myPosition = texture2DRect(positions, myIndices.xy);
	vec4 myRandoms =  texture2DRect(colors, myIndices.xy);
	vec4 myColor = texture2DRect(colors, myIndices.xy);// * gl_Color;
	vec4 myValues = texture2DRect(infos, myIndices.xy);
	
// 	myPosition = texture2DRect(positions, gl_Vertex.xy);
// 	myColor = texture2DRect(colors, gl_Vertex.xy);
	
	//vec4 myColor = gl_Color;
	//myColor.xyz +=myRandoms.x * 0.5;

	gl_Position = gl_ModelViewProjectionMatrix * myPosition;
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);
	
	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * myPosition;
	//gl_PointSize =  max(tanHalfFOV / -posViewSpace.z * pointSize * myRandoms.y, 1);
	gl_PointSize =  ((tanHalfFOV / -posViewSpace.z) * pointSize) * myValues.y;

	float myAlpha = clamp(1 - myValues.x / myValues.y,0,1);
	
// 	gl_FrontColor = min(myColor * gl_PointSize * gl_PointSize, myColor);
	gl_FrontColor = myColor;
	gl_FrontColor.a *= myAlpha;
// 	gl_FrontColor.a = 1;
// 	gl_FrontColor = vec4(0,1,0,1);
	
	float blurDepth =  3000;
    float depth = (-posViewSpace.z)/(blurDepth);
    float blur = 1.0 - clamp(depth, 0.0, 1.0);    
	gl_TexCoord[1] = vec4(blur,blur,blur,1); 
}
	           