// maximum size supported by this shader

// array of offsets for accessing the base image
uniform float2 offset[];

// value for each location in the convolution kernel
uniform float kernelValue[];

void main(
	in 		float2 		texCoord 	: TEXCOORD0,
	uniform sampler2D 	decal 		: TEX0,
	out 	float4		color 		: COLOR0
){
    float4 sum = float4 (0.0);

    for (int i = 0; i < kernelValue.length; i++){
        float4 tmp = tex2D(decal, texCoord + offset[i]);
        sum += tmp  * kernelValue[i];
    }
    color = sum;
}