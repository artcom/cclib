#version 120
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform float pointSize;
uniform float alphaFactor;

//uniform sampler2DRect springs;
uniform sampler2DRect positions;
uniform sampler2DRect infos;
uniform sampler2DRect colors;

void main ()
{
	vec4 myPosition = texture2DRect(positions, gl_Vertex.xy);
	gl_Position = gl_ModelViewProjectionMatrix * myPosition;
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0.0,0.0);

	vec4 myValues = texture2DRect(infos, gl_Vertex.xy);

	float myAlpha = clamp(1.0 - myValues.x / myValues.y * (1 - myValues.z), 0.5, 1.0); // <-
	gl_FrontColor = texture2DRect(colors, gl_Vertex.xy) * gl_Color;
// 	gl_FrontColor.a = 1;
	gl_FrontColor.a = myAlpha;

	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * myPosition;
	float myPointSize = max(tanHalfFOV / -posViewSpace.z * pointSize * myValues.y, 1.0);
//     float myPointSize = max(tanHalfFOV / -posViewSpace.z * myValues.y, 1) * pointSize;
	gl_PointSize = myPointSize;



    float blurDepth =  3000.0;
    float depth = (-posViewSpace.z)/(blurDepth);
    float blur = 1.0 - clamp(depth, 0.0, 1.0);
	gl_TexCoord[1] = vec4(blur,blur,blur,1.0);

    /*float far =  3000;//10000.0;
    float depth = (-posViewSpace.z)/(far); // will map near..far to 0..1
	gl_TexCoord[1] = vec4(depth,depth,depth,1); */
}
