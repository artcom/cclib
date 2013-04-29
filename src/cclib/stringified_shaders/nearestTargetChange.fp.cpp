/* Generated file, do not edit! */

#include "nearestTargetChange.fp.h"

char const* const nearestTargetChange_fp = 
   "void main(\n"
   "	in float4 iColor : TEXCOORD0,\n"
   "	out float4 oColor : COLOR\n"
   "){\n"
   "	if(iColor.a == 0)discard;\n"
   "	oColor = float4(iColor.xy,1,1);\n"
   "}\n"
   ;
