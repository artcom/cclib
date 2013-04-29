/* Generated file, do not edit! */

#include "nearestTargetChange.vp.h"

char const* const nearestTargetChange_vp = 
   "uniform samplerRECT particleTargetInfos;\n"
   "uniform float4x4 mvp : state.matrix.mvp;\n"
   "void main(\n"
   "	in float4 iPosition : POSITION,\n"
   "	out float4 oPosition :POSITION,\n"
   "	out float4 oColor : TEXCOORD0\n"
   "){\n"
   "	float4 targetPosition = texRECT(particleTargetInfos, iPosition.xy);\n"
   "	oPosition = mul(mvp, targetPosition);\n"
   "	oColor = iPosition;\n"
   "}\n"
   ;
