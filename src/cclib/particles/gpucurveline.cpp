
#include "gpucurveline.h"
#include <gl/texture2d.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>
#include <math/math.h>
#include <stringified_shaders/initvalue.fp.h>

using namespace cclib; 

#define SAMPLES 512

GPUCurveLine::GPUCurveLine() :
    GPUForce("PointCurveForceFieldFollow"),
    _myPrediction(Property_<float>::create("prediction", 0)),
    _myRadius(Property_<float>::create("radius", 1)), 
    _myMinX(Property_<float>::create("minX", 0)),
    _myMaxX(Property_<float>::create("maxX", 1)),
    _myOutputScale(Property_<float>::create("outputScale", 1)),

    _myPredictionParameter(0), 
    _myOutputScaleParameter(0), 
    _myMinXParameter(0),
    _myRangeXParameter(0),
    _myRadiusParmeter(0),
    _myCurveTextureParameter(0),

    _myCurveData(),
    _myInitValueShader()
{
    std::vector<cclib::Vector2f> points;
    for (int i=0; i<SAMPLES; i++) {
        points.push_back( cclib::Vector2f(i, 0) );
    }
    
    _myCurvePoints = Property_< std::vector<cclib::Vector2f> >::create("curvePoints", points);
    
    registerProperty(_myPrediction);
    registerProperty(_myRadius);
    registerProperty(_myMinX);
    registerProperty(_myMaxX);
    registerProperty(_myOutputScale);
    registerProperty(_myCurvePoints);

    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    ffiles.push_back(initvalue_fp);
        
    _myCurveData = cclib::ShaderBuffer::create(SAMPLES, 1, 32, 4, 1,  GL_TEXTURE_2D);
    _myCurveData->attachment(0)->textureFilter(GL_LINEAR);

    _myInitValueShader = cclib::CGShader::create(vfiles, ffiles); 
    _myInitValueShader->load();
}

GPUCurveLinePtr
GPUCurveLine::create() {
    return GPUCurveLinePtr(new GPUCurveLine());
}

void 
GPUCurveLine::setupParameter(int theWidth, int theHeight) {
    GPUForce::setupParameter(theWidth, theHeight);

    _myPredictionParameter = parameter("prediction");
    _myOutputScaleParameter = parameter("outputScale");
    _myMinXParameter = parameter("minX");
    _myRangeXParameter = parameter("rangeX");
    _myRadiusParameter = parameter("radius");
    _myCurveTextureParameter = parameter("curveData");
}

void 
GPUCurveLine::update(float theDeltaTime) {
    GPUForce::update(theDeltaTime);

    _myVelocityShader->parameter(_myRadiusParameter, _myRadius->getValue<float>());
    _myVelocityShader->parameter(_myPredictionParameter, _myPrediction->getValue<float>());
    _myVelocityShader->parameter(_myOutputScaleParameter, _myOutputScale->getValue<float>());
    _myVelocityShader->parameter(_myMinXParameter, _myMinX->getValue<float>());
    _myVelocityShader->parameter(_myRangeXParameter, _myMaxX->getValue<float>() - _myMinX->getValue<float>());
    _myVelocityShader->texture(_myCurveTextureParameter, _myCurveData->attachment(0)->id());
    
    _myCurveData->beginDraw();
    Graphics::clear();
    _myInitValueShader->start();
    
    glDisable (GL_BLEND);
    glDisable (GL_ALPHA_TEST);
    glDepthFunc (GL_LEQUAL);
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    
    Graphics::beginShape(GL_POINTS);
    
    std::vector<cclib::Vector2f> points = _myCurvePoints->getValue<std::vector<cclib::Vector2f> >();
    int csize = points.size();

    float minX = _myMinX->getValue<float>();
    float maxX = _myMaxX->getValue<float>();
    float range = maxX - minX;
    float x = minX;
    float xStep = range / csize;

    for(int i=0; i<csize; i++) {
        Graphics::textureCoords(0, x, points[i].x(), points[i].y());
        Graphics::vertex( i + 0.5f, 0.5f);
        x+=xStep;
    }

    Graphics::endShape();
    _myInitValueShader->end();
    _myCurveData->endDraw();
}


