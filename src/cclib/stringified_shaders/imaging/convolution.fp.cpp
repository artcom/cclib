/* Generated file, do not edit! */

#include "convolution.fp.h"

char const* const convolution_fp = 
   "// maximum size supported by this shader\n"
   "// array of offsets for accessing the base image\n"
   "uniform float2 offset[];\n"
   "// value for each location in the convolution kernel\n"
   "uniform float kernelValue[];\n"
   "void main(\n"
   "	in 		float2 		texCoord 	: TEXCOORD0,\n"
   "	uniform sampler2D 	decal 		: TEX0,\n"
   "	out 	float4		color 		: COLOR0\n"
   "){\n"
   "    float4 sum = float4 (0.0);\n"
   "    for (int i = 0; i < kernelValue.length; i++){\n"
   "        float4 tmp = tex2D(decal, texCoord + offset[i]);\n"
   "        sum += tmp  * kernelValue[i];\n"
   "    }\n"
   "    color = sum;\n"
   "}\n"
   ;
