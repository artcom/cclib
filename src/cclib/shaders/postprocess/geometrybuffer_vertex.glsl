#version 120

varying vec4 pos;
varying vec4 posinv;
varying vec3 normal;
varying float depth;

varying vec4 albedo;
varying vec4 diffuse;
varying vec4 specular;
   
uniform float near;
uniform float far;
uniform mat4 inverseView;

varying vec4 ambientGlobal, ambient;
varying vec3 lightDir,halfVector;
varying float dist;

// vec4 spot_lighting()
// {
// 	//vec3  lightPositionOC = gl_LightSource[0].position;   // in object coordinates
// 	//vec3  spotDirectionOC = gl_LightSource[0].position;   // in object coordinates
// 	float spotCutoff = gl_LightSource[0].spotCutoff;        // in degrees
// 
// 	vec3 lightPosition;
// 	vec3 spotDirection;
// 	vec3 lightDirection;
// 	float angle;
// 
// 	// Transforms light position and direction into eye coordinates
// 	//lightPosition  = (lightPositionOC * gl_ModelViewMatrix).xyz;
// 	//spotDirection  = normalize(spotDirectionOC * gl_NormalMatrix);
// 	
// 	lightPosition = gl_LightSource[0].position.xyz;
// 	spotDirection = gl_LightSource[0].spotDirection;
// 	
// 	// Calculates the light vector (vector from light position to vertex)
// 	vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
// 	lightDirection = normalize(vertex.xyz - lightPosition.xyz);
// 
// 	// Calculates the angle between the spot light direction vector and the light vector
// 	angle = dot( normalize(spotDirection),
// 			-normalize(lightDirection));
// 	angle = max(angle,0);   
// 
// 	// Test whether vertex is located in the cone
// 	if(angle > radians(spotCutoff))
// 		return vec4(1,1,0,1); // lit (yellow)
// 	else
// 		return vec4(0.5,0.5,0.5,1); // unlit
// }

void main(){
	pos	= gl_ModelViewMatrix * gl_Vertex;
	posinv = inverseView * pos;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	depth = 1-(-posinv.z - near) / (far - near);

	albedo = gl_FrontMaterial.diffuse;
	diffuse	= gl_FrontMaterial.diffuse;
	specular = gl_FrontMaterial.specular;
	specular.a = gl_FrontMaterial.shininess;
	//albedo = gl_FrontMaterial.emission + (gl_LightModel.ambient * gl_FrontMaterial.ambient);
	
	
	// https://github.com/silam/OutForDrive/blob/master/spotlight-vshader-transform.glsl
	
	vec3 aux = vec3(gl_LightSource[0].position-pos);
	lightDir = normalize(aux);
	/* compute the distance to the light source to a varying variable*/
	dist = length(aux);
	/* Normalize the halfVector to pass it to the fragment shader */
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	
	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position  = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
// 	gl_FrontColor = spot_lighting();
}