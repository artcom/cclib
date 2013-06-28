#version 120

varying vec4 albedo;
varying vec4 diffuse;
varying vec4 specular;
varying vec4 pos;
varying vec4 posinv;
varying vec3 normal;
varying float depth;

uniform sampler2D colorTexture; // GL_TEXTURE_2D
//uniform sampler2DRect colorTexture; // GL_TEXTURE_RECTANGLE

varying vec4 ambientGlobal, ambient;
varying vec3 lightDir,halfVector;
varying float dist;


vec4 generic_lighting(vec4 vertex_e, vec4 color, vec3 normal_e)
{
	// Return lighted color of vertex_e.
	//
	vec4 light_pos = gl_LightSource[0].position;
	vec3 v_vtx_light = light_pos.xyz - vertex_e.xyz;
	float d_n_ve = -dot(normal_e,vertex_e.xyz);
	float d_n_vl = dot(normal_e, normalize(v_vtx_light).xyz);
	bool same_sign = ( d_n_ve > 0 ) == ( d_n_vl > 0 );
	float phase_light = same_sign ? abs(d_n_vl) : 0;

	vec3 ambient_light = gl_LightSource[0].ambient.rgb;
	vec3 diffuse_light = gl_LightSource[0].diffuse.rgb;
	float dist = length(v_vtx_light);
	float distsq = dist * dist;
	float atten_inv =
	gl_LightSource[0].constantAttenuation +
	gl_LightSource[0].linearAttenuation * dist +
	gl_LightSource[0].quadraticAttenuation * distsq;
	vec4 lighted_color;
	lighted_color.rgb =
	color.rgb * gl_LightModel.ambient.rgb
	+ color.rgb * ( ambient_light + phase_light * diffuse_light ) / atten_inv;
	lighted_color.a = color.a;
	return lighted_color;
}

/*
#define MAX_LIGHTS 2 

vec4 multi_lighting()
{
	vec3 N = normal;
	vec3 v = pos.xyz;
	vec4 finalColour;

	for (int i=0; i<MAX_LIGHTS; i++)
	{
		vec3 L = normalize(gl_LightSource[i].position.xyz - v);   
		vec3 E = normalize(-v);
		vec3 R = normalize(-reflect(L,N));  
		vec4 Iamb = gl_FrontLightProduct[i].ambient;    
		vec4 Idiff = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);
		Idiff = clamp(Idiff, 0.0, 1.0);     
		vec4 Ispec = gl_FrontLightProduct[i].specular 
					* pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
		Ispec = clamp(Ispec, 0.0, 1.0);
		finalColour += Iamb + Idiff + Ispec;
	}
// 	gl_FragColor = gl_FrontLightModelProduct.sceneColor + finalColour;
	return finalColour; 
}
*/

// https://github.com/silam/OutForDrive/blob/master/spotlight-fshader-transform.glsl

vec4 spot_lighting(vec4 color)
{
	vec3 n,halfV;
	float NdotL,NdotHV;
	float att,spotEffect;

	/* a fragment shader can't write a verying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);

	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,normalize(lightDir)),0.0);

	if (NdotL > 0.0) {

		spotEffect = dot(normalize(gl_LightSource[0].spotDirection), normalize(-lightDir));
		if (spotEffect > gl_LightSource[0].spotCosCutoff) 
		{
			color = ambientGlobal;
		
			spotEffect = pow(spotEffect, gl_LightSource[0].spotExponent);
			att = spotEffect / (gl_LightSource[0].constantAttenuation +
								gl_LightSource[0].linearAttenuation * dist +
								gl_LightSource[0].quadraticAttenuation * dist * dist);

			color += att * (diffuse * NdotL + ambient);

			halfV = normalize(halfVector);
			//NdotHV = max(dot(n,halfV),0.0);
			NdotHV = dot(n,halfV);
			color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		}
		else
		{
			color = ambientGlobal;
		}
	}

	return color;
}

void main(){
	gl_FragData[0] = posinv;

	/*gl_FragData[1] = vec4(
		normal.x * 0.5f + 0.5f, 
		normal.y * 0.5f + 0.5f, 
		normal.z * 0.5f + 0.5f, 
		depth
	);*/	

	
	gl_FragData[1] = vec4(
		normal.x, 
		normal.y, 
		normal.z, 
		depth
	);
	

// 	gl_FragData[2] = gl_Color;
// 	gl_FragData[2] = gl_Color * texture2D(colorTexture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y));
	
	vec4 myColor = texture2D(colorTexture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y));

// 	vec4 myLightedColor = gl_Color * generic_lighting( pos, myColor, normalize(normal));
	
// 	gl_FragData[2] = myColor * multi_lighting();
	gl_FragData[2] = spot_lighting(myColor);
	
	
	//gl_FragData[2] = gl_Color * texture2D(colorTexture, vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y));
	//gl_FragData[2] = texture2DRect(colorTexture, gl_TexCoord[0].xy);
	
	
}