/* Generated file, do not edit! */

#include "scene.fp.h"

char const* const scene_fp = 
   "uniform sampler2D texture : TEXUNIT0;\n"
   "void main(\n"
   "	in 	float4 	iColor 	: COLOR,\n"
   "	in 	float2 	iTexCoord 	: TEXCOORD0,\n"
   "	in 	float 	iBlur	 : TEXCOORD1,\n"
   "	\n"
   "	out float4 	oColor 	: COLOR0,\n"
   "	out float4 	oBlur	: COLOR1\n"
   "){\n"
   "	oColor = iColor * float4(tex2D(texture, iTexCoord).xyz, 1);\n"
   "	oBlur = iBlur;\n"
   "}\n"
   ;
