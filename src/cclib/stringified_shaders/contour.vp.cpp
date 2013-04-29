/* Generated file, do not edit! */

#include "contour.vp.h"

char const* const contour_vp = 
   "uniform float4x4 modelViewProjection : state.matrix.mvp;\n"
   "void main(\n"
   "	in float4 iPosition : POSITION,\n"
   "	in float3 iNormal : NORMAL,\n"
   "	out float4 oPosition : POSITION,\n"
   "	out float4 oColor : COLOR\n"
   "){\n"
   "	oColor = float4(iNormal.xyz / 2 +0.5,1);\n"
   "	oPosition = mul(modelViewProjection, iPosition);\n"
   "}\n"
   ;
