/* Generated file, do not edit! */

#include "downsample.fp.h"

char const* const downsample_fp = 
   "uniform sampler2D texture : TEXUNIT0;\n"
   "void main(\n"
   "	in 	float2 	iTexCoord 	: TEXCOORD0,\n"
   "	\n"
   "	out float4 	oColor 	: COLOR0\n"
   "){\n"
   "	oColor = tex2D(texture, iTexCoord);\n"
   "}\n"
   ;
