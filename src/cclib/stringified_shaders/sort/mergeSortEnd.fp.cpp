/* Generated file, do not edit! */

#include "mergeSortEnd.fp.h"

char const* const mergeSortEnd_fp = 
   "uniform samplerRECT sortData : TEXUNIT0;\n"
   "uniform int2 size;\n"
   "uniform int sortStep;\n"
   "	\n"
   "void main(\n"
   "	in float2 current : TEXCOORD0,\n"
   "	out float3 oColor : COLOR0\n"
   ") {\n"
   "	float3 currentSample = texRECT(sortData, current);\n"
   "	\n"
   "	float i = (current.y - 0.5) * size.x + current.x;\n"
   "	float b = (fmod(i / sortStep, 2.0) < 1.0 ? 1.0 : -1.0);\n"
   "	float otherI = i + (b * sortStep);\n"
   "	float2 otherPos = float2(fmod(otherI, size.x), floor(otherI / size.x) + 0.5);\n"
   "	float3 otherSample = texRECT(sortData, otherPos);\n"
   "	if (b >= 0){\n"
   "		oColor = currentSample.z < otherSample.z ? currentSample : otherSample;\n"
   "	} else {\n"
   "		oColor = currentSample.z > otherSample.z ? currentSample : otherSample;\n"
   "	}\n"
   "}\n"
   ;
