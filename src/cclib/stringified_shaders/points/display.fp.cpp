/* Generated file, do not edit! */

#include "display.fp.h"

char const* const display_fp = 
   "uniform samplerRECT infoTexture : TEXUNIT0;\n"
   "void main(\n"
   "	in float4 iColor : COLOR,\n"
   "	in float2 iTexCoord : TEXCOORD0,\n"
   "	out float4 oColor : COLOR0\n"
   "){\n"
   "	float4 myValues = texRECT(infoTexture, iTexCoord);\n"
   "	float myAlpha = saturate(1 - myValues.x / myValues.y);\n"
   "	//oColor = float4(iColor.xyz, iColor.w * pow(myAlpha,4));\n"
   "	oColor = iColor;\n"
   "	//oColor = float4(1,1,1, 1);\n"
   "}\n"
   ;
