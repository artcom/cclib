/* Generated file, do not edit! */

#include "computeDistance.fp.h"

char const* const computeDistance_fp = 
   "uniform samplerRECT sortTexture : TEXUNIT0;\n"
   "uniform samplerRECT positionTexture : TEXUNIT1;\n"
   "uniform float3 viewerPosition;\n"
   "void main(\n"
   "	in float2 iTexCoord : TEXCOORD0,\n"
   "	out float4 oColor : COLOR\n"
   "){\n"
   "	float2 particleIndex = texRECT(sortTexture, iTexCoord.xy).xy;\n"
   "	float3 particlePos = (float3)texRECT(positionTexture, particleIndex);\n"
   "	float3 delta = viewerPosition - particlePos;\n"
   "	float distanceSqr = dot(delta, delta);\n"
   "	// Prevent unused, far-away particles from destroying comparisons in sorting.\n"
   "	if (distanceSqr > 1e6 || isnan(distanceSqr))\n"
   "		distanceSqr = 1e6;\n"
   "	oColor = float4(particleIndex,particlePos.z,1.0);\n"
   "}\n"
   ;
