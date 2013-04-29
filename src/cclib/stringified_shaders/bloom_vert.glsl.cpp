/* Generated file, do not edit! */

#include "bloom_vert.glsl.h"

char const* const bloom_vert_glsl = 
   "void main(){\n"
   "	gl_Position = ftransform();\n"
   "	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"
   "}\n"
   ;
