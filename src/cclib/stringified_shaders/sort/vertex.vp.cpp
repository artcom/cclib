/* Generated file, do not edit! */

#include "vertex.vp.h"

char const* const vertex_vp = 
   "uniform float4x4 modelViewProjection : state.matrix.mvp;\n"
   "uniform float4x4 modelView : state.matrix.modelview[0];\n"
   "void main (\n"
   "	in float4 iPosition : POSITION,\n"
   "	in float2 iTexCoord : TEXCOORD0,\n"
   "	in float4 iColor : COLOR0,\n"
   "	out float4 oPosition : POSITION,\n"
   "	out float2 oTexCoord : TEXCOORD0,\n"
   "	out float4 oColor : COLOR0\n"
   "){\n"
   "	oPosition = mul(modelViewProjection,iPosition);\n"
   "	oTexCoord = iTexCoord;\n"
   "	oColor = iColor;\n"
   "}\n"
   "	           \n"
   ;
