#version 120 
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect colorBuffer;

uniform float minPointSize;
uniform float maxPointSize;
uniform float pointSize;
uniform float tanHalfFov;

void main (){
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec4 posViewSpace = gl_ModelViewMatrix * gl_Vertex;
	//oPointSize = clamp(pointSize * tanHalfFov / -oPosition.z, minPointSize, maxPointSize);
	// Compute point size.
	
	//const float fieldOfView = radians(60.0);
	//pointSize = 50 * tan(fieldOfView / 2) / -posViewSpace.z;
	
	float myPointSize = tanHalfFov / -posViewSpace.z;

	// Do fake antialiasing if point size is below 1.	
	/*o.Color = color;
	o.PointSize = pointSize;
	if (pointSize < 1)
	{
		o.Color = color * pointSize; /// BUG: Squared!
		o.PointSize = 1;
	}*/
	gl_FrontColor = gl_Color;//min(gl_Color * pointSize * pointSize, gl_Color);
	gl_PointSize = max(myPointSize, 1) * pointSize;
}
	           