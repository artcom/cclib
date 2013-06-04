
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
    
//    _myColorTextureParameter = vertexParameter("colorTexture");
    
    load();

    // XXX pointsize not quite working yet [sh].
    setPointSize(1.0);
    setMinPointSize(0.00001f);
    setMaxPointSize(2.5f);
//    setColorBufferTarget(0);
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
GPUDisplayShader::setPointSize(float thePointSize) {
    parameter(_myPointSizeParameter, thePointSize);
}

void 
GPUDisplayShader::setMinPointSize(float theMinPointSize) {
    parameter(_myMinPointSizeParameter, theMinPointSize);
}

void 
GPUDisplayShader::setMaxPointSize(float theMaxPointSize) {
    parameter(_myMaxPointSizeParameter, theMaxPointSize);
}

void 
GPUDisplayShader::setTangHalfFov(float theTangHalfFov) {
    parameter(_myTangHalfFovParameter, theTangHalfFov);
}

//void
//GPUDisplayShader::setColorTexture(int theID)
//{
//    texture(_myColorTextureParameter, theID);
//}
