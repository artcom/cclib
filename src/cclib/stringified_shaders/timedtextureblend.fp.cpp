/* Generated file, do not edit! */

#include "timedtextureblend.fp.h"

char const* const timedtextureblend_fp = 
   "uniform samplerRECT positions;\n"
   "uniform samplerRECT blendTexture;\n"
   "uniform samplerRECT oldBlends;\n"
   "uniform float2 textureScale;\n"
   "uniform float2 textureOffset;\n"
   "uniform float deltaTime;\n"
   "void main(\n"
   "	in float2 wPos : WPOS,\n"
   "	out float4 oColor : COLOR\n"
   "){\n"
   "	float3 position = texRECT(positions, wPos);\n"
   "	float2 texturePos = (position.xy * float2(1,1)) / textureScale + textureOffset;\n"
   "	\n"
   "	float oldBlend = texRECT(oldBlends, wPos);\n"
   "	oColor = float4(saturate(oldBlend + (texRECT(blendTexture, texturePos.xy).x - 0.5) * 2 * deltaTime),0,0,1);\n"
   "	//oColor = float4(position.x / 1200.0 + 0.5,position.y/600.0+0.5,0,1);\n"
   "}\n"
   ;
