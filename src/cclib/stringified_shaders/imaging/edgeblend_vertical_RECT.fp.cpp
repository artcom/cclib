/* Generated file, do not edit! */

#include "edgeblend_vertical_RECT.fp.h"

char const* const edgeblend_vertical_RECT_fp = 
   "uniform samplerRECT blendTexture : TEXUNIT0;\n"
   "uniform float blendStart;\n"
   "uniform float blendEnd;\n"
   "uniform float blendPower;\n"
   "uniform float blendBreak;\n"
   "uniform float rGamma;\n"
   "uniform float gGamma;\n"
   "uniform float bGamma;\n"
   "void main(\n"
   "	in float2 iTexCoord : TEXCOORD0,\n"
   "	out float4 oColor : COLOR0\n"
   "){\n"
   "	float blend = (iTexCoord.y - blendStart) / (blendEnd - blendStart);\n"
   "	float blendStep = step(0.5, blend);\n"
   "	blend = \n"
   "	blendBreak * pow(2 * blend,blendPower) * (1 - blendStep)  + \n"
   "	(1 - (1 - blendBreak) * pow(2 * (1 - blend),blendPower)) * blendStep;\n"
   "	\n"
   "	float4 blendColor = float4(\n"
   "		pow(blend, 1 / rGamma),\n"
   "		pow(blend, 1 / gGamma),\n"
   "		pow(blend, 1 / bGamma),\n"
   "		1\n"
   "	);\n"
   "	float4 color = texRECT(blendTexture, iTexCoord);\n"
   "	oColor = color * blendColor;\n"
   "}\n"
   ;
