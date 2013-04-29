/* Generated file, do not edit! */

#include "indexed_display_vertex.glsl.h"

char const* const indexed_display_vertex_glsl = 
   "#version 120 \n"
   "#extension GL_ARB_texture_rectangle : enable\n"
   "uniform float tanHalfFOV;\n"
   "uniform sampler2DRect springs;\n"
   "uniform sampler2DRect positions;\n"
   "uniform sampler2DRect infos;\n"
   "uniform sampler2DRect colors;\n"
   "void main (){\n"
   "	vec4 myPosition = texture2DRect(positions, gl_Vertex.xy);\n"
   "	gl_Position = gl_ModelViewProjectionMatrix * myPosition;\n"
   "	gl_TexCoord[0] = vec4(gl_Vertex.xy,0,0);\n"
   "	\n"
   "	// Compute point size.\n"
   "	\n"
   "	vec4 myValues = texture2DRect(infos, gl_Vertex.xy);\n"
   "	float myAlpha = clamp(1 - myValues.x / myValues.y * (1 - myValues.z),0,1);\n"
   "	 \n"
   "	gl_FrontColor = texture2DRect(colors, gl_Vertex.xy) * gl_Color;\n"
   "	gl_FrontColor.a *= myAlpha * myAlpha;\n"
   "	\n"
   "	\n"
   "}\n"
   "	           \n"
   ;
