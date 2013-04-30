/* Generated file, do not edit! */

#include "downsample.vp.h"

char const* const downsample_vp = 
   "uniform float4x4 modelViewProjection : state.matrix.mvp;\n"
   "void main(\n"
   "	in 	float4 	iPosition	: POSITION,\n"
   "	in  float2  iTexCoord	: TEXCOORD0,\n"
   "	\n"
   "	out float4 	oPosition	: POSITION,\n"
   "	out float2 	oTexCoord	: TEXCOORD0\n"
   "){\n"
   "	oPosition = mul(modelViewProjection, iPosition);\n"
   "	oTexCoord = iTexCoord;\n"
   "}\n"
   ;
