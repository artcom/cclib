
#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform sampler2DRect positions;
uniform sampler2DRect infos;
uniform sampler2DRect indices;
uniform sampler2DRect colors;

uniform float pointSize;

void main (){
	vec4 myIndices = texture2DRect(indices, gl_Vertex.xy);

	vec4 myPosition = texture2DRect(positions, myIndices.xy);

	vec4 myRandoms =  texture2DRect(colors, myIndices.xy);
	vec4 myColor = texture2DRect(colors, myIndices.xy) * gl_Color;

	vec4 myValues = texture2DRect(infos, myIndices.xy);

	//vec4 myColor = gl_Color;
	//myColor.xyz +=myRandoms.x * 0.5;

	gl_Position = gl_ModelViewProjectionMatrix * myPosition;
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);
	
	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * myPosition;
	//gl_PointSize =  max(tanHalfFOV / -posViewSpace.z * pointSize * myRandoms.y, 1);
	gl_PointSize =  ((tanHalfFOV / -posViewSpace.z) * pointSize) * myValues.y;

	
	float myAlpha = clamp(1 - myValues.x / myValues.y,0,1);
	
	//gl_FrontColor = min(myColor * gl_PointSize * gl_PointSize, myColor);
	//gl_FrontColor.a *= myAlpha;
	gl_FrontColor = vec4(1,0,0,1);
}


/*
#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform sampler2DRect positions;
uniform sampler2DRect indices;
uniform sampler2DRect infos;
uniform sampler2DRect colors;

uniform float pointSize;
uniform float minPointSize;
uniform float maxPointSize;

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
	float myPointSize =  max(tanHalfFOV / -posViewSpace.z * pointSize, 1) * myRandoms.y;
	//myPointSize = clamp(myPointSize, minPointSize, maxPointSize);
	gl_PointSize = myPointSize;

	vec4 myValues = texture2DRect(infos, myIndices.xy);
	float myAlpha = clamp(1 - myValues.x / myValues.y,0,1);
	
	gl_FrontColor = min(myColor * gl_PointSize * gl_PointSize, myColor);
	gl_FrontColor.a *= myAlpha * myAlpha;
	//gl_FrontColor = vec4(1,0,0,1);

}
*/

/*
#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform float tanHalfFOV;

uniform float pointSize;
uniform float minPointSize;
uniform float maxPointSize;

uniform sampler2DRect positions;
uniform sampler2DRect indices;
uniform sampler2DRect infos;

uniform vec3 planeNormal;
uniform float planeConstant;
uniform vec3 lightDirection;

vec3 intersection(vec3 thePosition, vec3 theDirection) {
	float denominator = dot(planeNormal, theDirection);

	float numerator = -(dot(planeNormal, thePosition) - planeConstant);
	float ratio = numerator / denominator;

	if (ratio < 0)
		return vec3(10000000,0,0); // intersects behind origin

	return theDirection * ratio + thePosition;
}

void main (){
	vec4 myIndices = texture2DRect(indices, gl_Vertex.xy);
	vec4 myPosition = texture2DRect(positions, myIndices.xy);
	
	vec3 direction = normalize(myPosition.xyz - lightDirection);
	vec3 myShadowPosition = intersection(myPosition.xyz, direction);
	gl_Position = gl_ModelViewProjectionMatrix * vec4(myShadowPosition,1);
	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);
	
	// Compute point size.
	vec4 posViewSpace = gl_ModelViewMatrix * vec4(myShadowPosition,1);
	gl_PointSize = 2;//max(tanHalfFOV / -posViewSpace.z * pointSize, 1);
	
	vec4 myValues = texture2DRect(infos, myIndices.xy);
	float myAlpha = clamp(1 - myValues.x / myValues.y,0,1);
	
	gl_FrontColor = min(gl_Color * gl_PointSize * gl_PointSize, gl_Color);
	gl_FrontColor.a *= myAlpha * myAlpha;
}
*/
	           