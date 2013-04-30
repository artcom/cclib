/* Generated file, do not edit! */

#include "colortransform.fp.h"

char const* const colortransform_fp = 
   "uniform sampler2D texture : TEX0;\n"
   "uniform float4x4 colorMatrix;\n"
   "void main(\n"
   "	in float2 texCoord : TEXCOORD0,\n"
   "	out float3 color : COLOR0\n"
   "){\n"
   "    color = mul(colorMatrix,tex2D(texture, texCoord));\n"
   "}\n"
   ;
