#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform float pointSize;
uniform float minPointSize;
uniform float maxPointSize;

//uniform sampler2DRect springs;
uniform sampler2DRect positions;
uniform sampler2DRect infos;
uniform sampler2DRect colors;

void main ()
{
	vec4 myPosition = texture2DRect(positions, gl_Vertex.xy);
	gl_Position = gl_ModelViewProjectionMatrix * myPosition;
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);
	
	vec4 myValues = texture2DRect(infos, gl_Vertex.xy);

	//float myAlpha = clamp(1 - myValues.x / myValues.y * (1 - myValues.z), 0.2, 1); // <-
	float myAlpha = clamp(1 - myValues.x / myValues.y, 0.5, 1); 
	//myAlpha = 0.5;
	//float myAlpha = clamp(myValues.x, 0.2, 1); 

	gl_FrontColor = texture2DRect(colors, gl_Vertex.xy) * gl_Color;
	gl_FrontColor.a = 1;
	//gl_FrontColor.a = myAlpha;
	//gl_FrontColor.a *= myAlpha * myAlpha;
	//gl_FrontColor.a = (1. - myAlpha);
	


	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * myPosition;
	//vec4 posViewSpace = gl_ModelViewMatrix * gl_Position;
	//vec4 posViewSpace = gl_ModelViewMatrix * gl_Vertex;
	//vec4 posViewSpace = gl_ModelViewProjectionMatrix * myPosition;
	//vec4 posViewSpace = gl_ModelViewProjectionMatrix * gl_Vertex;

	//float tanHalfFOV_ = tan(radians(20.0) / 2.0);

	float z = (posViewSpace.z == 0) ? -1 : posViewSpace.z; 
	//float myPointSize = ((tanHalfFOV / -z) * pointSize) * myValues.x; // <-
	//float myPointSize = ((tanHalfFOV / -z) * pointSize) * (myValues.x / 2.); // <-
	//float myPointSize = ((tanHalfFOV / -posViewSpace.z) * pointSize) * myValues.y;
	//float myPointSize = ((tanHalfFOV / -posViewSpace.z) * pointSize) * myValues.z;


	//float myPointSize = (tanHalfFOV / posViewSpace.z) * pointSize;
	//float myPointSize = max(tanHalfFOV / -posViewSpace.z * pointSize, 1) * myValues.y;
	float myPointSize = max(tanHalfFOV / -posViewSpace.z * pointSize * myValues.y, 1);

	//float fieldOfView = radians(20.0);
	//float myPointSize =  ( (tan(fieldOfView / 2) / -posViewSpace.z) * pointSize ) * myValues.y;


	//float myPointSize = -posViewSpace.z;
	//myPointSize = myPosition.z;
	//myPointSize = clamp(myPointSize, minPointSize, maxPointSize);

	//myPointSize = 2;
	//gl_PointSize = myPointSize;
	gl_PointSize = myPointSize;

	//float myAlpha = clamp(myPointSize, 1, 0.2);
	//gl_FrontColor.a = myAlpha;

	//gl_FrontColor = min(gl_Color * gl_PointSize * gl_PointSize, gl_Color);
}
	           