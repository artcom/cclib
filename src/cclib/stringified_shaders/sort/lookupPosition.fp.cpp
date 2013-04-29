/* Generated file, do not edit! */

#include "lookupPosition.fp.h"

char const* const lookupPosition_fp = 
   "uniform samplerRECT sortTexture : TEXUNIT0;\n"
   "uniform samplerRECT positionTexture : TEXUNIT1;\n"
   "void main(\n"
   "	in float2 iTexCoord : TEXCOORD0,\n"
   "	out float4 oColor : COLOR0\n"
   "){\n"
   "	float2 sortIndex = texRECT(sortTexture, iTexCoord.xy).y;\n"
   "	half2 particleIndex = unpack_2half(sortIndex);\n"
   "	oColor = (float4)texRECT(positionTexture, particleIndex);\n"
   "}\n"
   ;
