/* Generated file, do not edit! */

#include "scene.vp.h"

char const* const scene_vp = 
   "uniform float4x4 modelViewProjection : state.matrix.mvp;\n"
   "uniform float4x4 modelView : state.matrix.modelview[0];\n"
   "uniform float focalDistance;\n"
   "uniform float focalRange;\n"
   "void main(\n"
   "	in 	float4 	iPosition	: POSITION,\n"
   "	in  float2  iTexCoord	: TEXCOORD0,\n"
   "	in 	float4 	iColor		: COLOR,\n"
   "	\n"
   "	out float4 	oPosition	: POSITION,\n"
   "	out float2 	oTexCoord	: TEXCOORD0,\n"
   "	out float4 	oColor		: COLOR,\n"
   "	out float 	blur		: TEXCOORD1\n"
   "){\n"
   "  	\n"
   "  	\n"
   "	oPosition = mul(modelViewProjection, iPosition);\n"
   "	oTexCoord = iTexCoord;\n"
   "	oColor = iColor;\n"
   "	\n"
   "	float4 PosWV = mul(modelView, iPosition);\n"
   "	blur = clamp(abs(-PosWV.z - focalDistance) / focalRange, 0.0, 1.0);\n"
   "}\n"
   ;
