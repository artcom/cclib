//
//  gpuparticlesort.cpp
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#include "gpuparticlesort.h"
#include "gl/graphics.h"
#include "gl/shaderbuffer.h"
#include "gl/cgshader.h"
#include "particles/gpuparticles.h"
#include "stringified_shaders/sort/mergeSortRecursion.fp.h"
#include "stringified_shaders/sort/mergeSortEnd.fp.h"
#include "stringified_shaders/sort/initSortIndex.fp.h"
#include "stringified_shaders/sort/computeDistance.fp.h"
#include "stringified_shaders/sort/lookupPosition.fp.h"
#include "stringified_shaders/sort/vertex.vp.h"

//cclib::GPUParticleSort::GPUParticleSort()
//{
//    printf("%s\n",__PRETTY_FUNCTION__);
//}

cclib::GPUParticleSort::GPUParticleSort(cclib::GPUParticles* theParticles)
{
//    _myGraphics = theGraphics;
    _myParticles = cclib::GPUParticlesPtr(theParticles);
 
    printf("%s\n\tparticles %d x %d\n",__PRETTY_FUNCTION__,_myParticles->width(),_myParticles->height());
    
    _myBuffer = cclib::ShaderBuffer::create(_myParticles->width(),_myParticles->height(),32, 4);
    _myDestinationBuffer = cclib::ShaderBuffer::create(_myParticles->width(),_myParticles->height(),32, 4);

    std::vector<std::string> vfiles, ffiles;
    
    ffiles.push_back(std::string(mergeSortRecursion_fp));
    _mySortRecursionShader = cclib::CGShader::create(
                                                     //NULL, CCIOUtil.classPath(this,"shader/sort/mergeSortRecursion.fp")
                                                     vfiles,ffiles);
    _mySortRecursionShaderSizeParameter = _mySortRecursionShader->fragmentParameter("size");
    _mySortRecursionShaderSortStepParameter = _mySortRecursionShader->fragmentParameter("sortStep");
    _mySortRecursionShaderSortCountParameter = _mySortRecursionShader->fragmentParameter("sortCount");
    _mySortRecursionShader->load();

    vfiles.clear();
    ffiles.clear();
    ffiles.push_back(std::string(mergeSortEnd_fp));
    _mySortEndShader = cclib::CGShader::create(
                                               //null, CCIOUtil.classPath(this, "shader/sort/mergeSortEnd.fp")
                                               vfiles,ffiles);
    _mySortEndShaderSizeParameter = _mySortEndShader->fragmentParameter("size");
    _mySortEndShaderSortStepParameter = _mySortEndShader->fragmentParameter("sortStep");
    _mySortEndShader->load();

    vfiles.clear();
    ffiles.clear();
    ffiles.push_back(std::string(initSortIndex_fp));
    _myDistanceSortInitShader = cclib::CGShader::create(
                                                        //null, CCIOUtil.classPath(this, "shader/sort/initSortIndex.fp")
                                                        vfiles,ffiles);
    _myDistanceSortInitShader->load();

    vfiles.clear();
    ffiles.clear();
    vfiles.push_back(std::string(vertex_vp));
    ffiles.push_back(std::string(computeDistance_fp));
    _myComputeDistanceShader = cclib::CGShader::create(
                                                       //CCIOUtil.classPath(this, "shader/sort/vertex.vp"), CCIOUtil.classPath(this, "shader/sort/computeDistance.fp")
                                                       vfiles,ffiles);
    _myComputeDistanceShader->load();
    
   vfiles.clear();
   ffiles.clear();
   ffiles.push_back(std::string(lookupPosition_fp));
   _myLookupPositionPositionShader = cclib::CGShader::create(
                                                             //null, CCIOUtil.classPath(this, "shader/sort/lookupPosition.fp")
                                                             vfiles,ffiles);
   _myLookupPositionPositionShader->load();
    
    _mySortPassesPerFrame = 5;
    
    reset();
}

void cclib::GPUParticleSort::reset()
{
    cclib::Graphics::noBlend();
    
    _myBuffer->beginDraw();
    cclib::Graphics::clear();
    
//    // DrawImage();
    _myBuffer->endDraw();

    _myCurrentPass = 0;
    _myMaxSortPasses = 100000;
    
    _myDistanceSortInitShader->start();
    _myBuffer->draw();
    _myDistanceSortInitShader->end();
    
//    if(DEBUG){
//        CCLog.info("RESET");
//        FloatBuffer myData = _myBuffer.getData();
//        while(myData.hasRemaining()){
//            CCLog.info(myData.get()+":"+myData.get()+":"+myData.get());
//        }
//    }
}

cclib::ShaderBufferPtr cclib::GPUParticleSort::indices(){
    return _myBuffer;
}

void cclib::GPUParticleSort::update(float theDeltaTime)
{
    cclib::Graphics::noBlend();
    // Update distances in sort texture.
    _myComputeDistanceShader->start();
    cclib::Graphics::texture(0, _myBuffer->attachment(0));
    cclib::Graphics::texture(1, _myParticles->dataBuffer()->attachment(0));
    _myDestinationBuffer->draw();
    cclib::Graphics::noTexture();
    _myComputeDistanceShader->end();
    
    cclib::ShaderBufferPtr myTmp = _myDestinationBuffer;
    _myDestinationBuffer = _myBuffer;
    _myBuffer = myTmp;
    
    mergeSort();
}

void cclib::GPUParticleSort::mergeSort()
{
    _myCurrentPass = 0;
    
    int logdSize = (int) log2(_myParticles->size());
    _myMaxSortPasses = (logdSize + 1) * logdSize / 2;
    
    _myBeginPass = _myEndPass;
    _myEndPass = (_myBeginPass + _mySortPassesPerFrame) % _myMaxSortPasses;
    
    _mySortRecursionShader->parameter(_mySortRecursionShaderSizeParameter, _myParticles->width(), _myParticles->height());
    _mySortEndShader->parameter(_mySortEndShaderSizeParameter, _myParticles->width(), _myParticles->height());
    
    doMergeSortPass(_myParticles->size());
}

void cclib::GPUParticleSort::doMergeSortPass(int theCount)
{
//    if (DEBUG)
//        CCLog.info("mergeSort: count=" + theCount);
    
    if (theCount > 1) {
        doMergeSortPass(theCount / 2);
        doMergePass(theCount, 1);
    }
    
//    if (DEBUG)
//        CCLog.info("mergeSort: end");
}

bool cclib::GPUParticleSort::doNextPass(){
    
    if (_myBeginPass < _myEndPass) {
        if (_myCurrentPass < _myBeginPass || _myCurrentPass >= _myEndPass){
            return false;
        }
    } else {
        if (_myCurrentPass < _myBeginPass && _myCurrentPass >= _myEndPass){
            return false;
        }
    }
    return true;
}

void cclib::GPUParticleSort::doMergePass(int theCount, int theStep)
{
    if (theCount > 2) {
        doMergePass(theCount / 2, theStep * 2);
        
        _myCurrentPass++;
        
        if(!doNextPass())return;
        
//        if (DEBUG)
//            CCLog.info(_myCurrentPass + ": mergeRec: count=" + theCount + ", step=" + theStep);
        
        _mySortRecursionShader->start();
        _mySortRecursionShader->parameter(_mySortRecursionShaderSortStepParameter, (float) theStep);
        _mySortRecursionShader->parameter(_mySortRecursionShaderSortCountParameter, (float) theCount);
        
        cclib::Graphics::texture(_myBuffer->attachment(0));
        _myDestinationBuffer->draw();
        cclib::Graphics::noTexture();
        _mySortRecursionShader->end();
        
        cclib::ShaderBufferPtr temp = _myBuffer;
        _myBuffer = _myDestinationBuffer;
        _myDestinationBuffer = temp;
        
        
    } else {
        _myCurrentPass++;
        
        if(!doNextPass())return;
        
//        if (DEBUG)
//            CCLog.info(_myCurrentPass + ": mergeEnd: count="+theCount+", step="+theStep);
        
        _mySortEndShader->start();
        _mySortEndShader->parameter(_mySortEndShaderSortStepParameter, (float) theStep);
        
        cclib::Graphics::texture(_myBuffer->attachment(0));
        _myDestinationBuffer->draw();
        cclib::Graphics::noTexture();
        
        _mySortEndShader->end();
        
        cclib::ShaderBufferPtr temp = _myBuffer;
        _myBuffer = _myDestinationBuffer;
        _myDestinationBuffer = temp;
        
        
    }
    
//    if(DEBUG){
//        //			FloatBuffer myData = _myBuffer.getData();
//        //			while(myData.hasRemaining()){
//        //				System.out.println(myData.get()+":"+myData.get()+":"+myData.get());
//        //			}
//    }
}
