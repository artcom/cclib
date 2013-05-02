
#include "gpudisplayshader.h"
#include <gl/cgshader.h>

using namespace cclib;

GPUDisplayShader::GPUDisplayShader(const std::vector<std::string> & theVertexFile, const std::vector<std::string> & theFragmentFile) :
    CGShader(theVertexFile, theFragmentFile)
{
    _myPointSizeParameter = vertexParameter("pointSize");
    _myMinPointSizeParameter = vertexParameter("minPointSize");
    _myMaxPointSizeParameter = vertexParameter("maxPointSize");

    _myTangHalfFovParameter = vertexParameter("tanHalfFov");
    load();

    // XXX pointsize not quite working yet [sh].
    pointSize(1.0);
    minPointSize(0.00001f);
    maxPointSize(2.5f);
    
//    pointSize(10.0);
//    minPointSize(5.00001f);
//    maxPointSize(20.5f);
}

GPUDisplayShaderPtr 
GPUDisplayShader::create(const std::string & theVertexFile, const std::string & theFragmentFile) 
{
    std::vector<std::string> vFiles; 
    std::vector<std::string> fFiles; 
    vFiles.push_back(theVertexFile);
    fFiles.push_back(theFragmentFile);
    
    return GPUDisplayShaderPtr(new GPUDisplayShader(vFiles, fFiles));     
}

void 
GPUDisplayShader::pointSize(float thePointSize) {
    parameter(_myPointSizeParameter, thePointSize);
}

void 
GPUDisplayShader::minPointSize(float theMinPointSize) {
    parameter(_myMinPointSizeParameter, theMinPointSize);
}

void 
GPUDisplayShader::maxPointSize(float theMaxPointSize) {
    parameter(_myMaxPointSizeParameter, theMaxPointSize);
}

void 
GPUDisplayShader::tangHalfFov(float theTangHalfFov) {
    parameter(_myTangHalfFovParameter, theTangHalfFov);
}

