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
: _myCurrentPass(0),_myBeginPass(0),_myEndPass(0)
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
    cclib::Graphics::clearColor(0,1,0,1);
    cclib::Graphics::clear();
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

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

    printf("mergeSort: current %10d, Begin  %10d, End  %10d, PassesPerFrame  %10d, MaxSortPasses  %10d\n",_myCurrentPass,_myBeginPass,_myEndPass,_mySortPassesPerFrame,_myMaxSortPasses);

    _mySortRecursionShader->parameter(_mySortRecursionShaderSizeParameter, _myParticles->width(), _myParticles->height());
    _mySortEndShader->parameter(_mySortEndShaderSizeParameter, _myParticles->width(), _myParticles->height());

    doMergeSortPass(_myParticles->size());
}

void cclib::GPUParticleSort::doMergeSortPass(int theCount)
{
//    if (DEBUG)
//        CCLog.info("mergeSort: count=" + theCount);

#if 0
    // original

    if(theCount > 1) {
        printf("\tdoMergeSortPass: count %10d\n", theCount);
        doMergeSortPass(theCount/2);
//      printf("\tdoMergePass, count %10d, step %10d\n", theCount, 1);
        doMergePass(theCount, 1);
    }

#else

    // refactored

#if 1

    for(int c = 2; c < theCount; c <<= 1) {
        printf("\tdoMergeSortPass: count %10d/%d\n", c, theCount);
        doMergePass(c, 1);
    }

    doMergePass(theCount, 1);

#else

    for(int i = 2; i < theCount; i <<= 1) {
        printf("\tdoMergeSortPass: count %10d\n", i);

        int count = i;
        int step = 1;
        int failed = 0;

        while(2 < count) {

            printf("\t\tdoMergePass current %10d, count %10d, step %10d\n", _myCurrentPass, count, step);

            count >>= 2;
            step <<= 2;

            if(!doNextPass()) {
                printf(".");
                failed++;
            }

            tail_shader(count, step);

        } // for j

        printf("\t\tdoMergePass current %10d, count %10d, step %10d\n", _myCurrentPass,count, step);

        head_shader(step);

        _myCurrentPass += failed;

    } // for i

    printf("\tdoMergeSortPass: count %10d\n", theCount);

#endif

#endif


//
//  doMergePass(theCount, 1);

/*
    printf("\tdoMergeSortPass: count %10d\n",theCount);
 *    if (theCount > 1) {
        doMergeSortPass(theCount / 2);
        doMergePass(theCount, 1);
    }
*/

//    if (DEBUG)
//        CCLog.info("mergeSort: end");
}

bool cclib::GPUParticleSort::doNextPass() {

    int rc = true;

    if (_myBeginPass < _myEndPass) {
        if (_myCurrentPass < _myBeginPass ||  _myEndPass < _myCurrentPass) {
            rc = false;
        }
    } else {
        if (_myEndPass < _myCurrentPass && _myCurrentPass < _myBeginPass) {
            rc = false;
        }
    }

//  printf("\tnext pass? %8d %8d %8d | %d\n", _myBeginPass, _myEndPass, _myCurrentPass, rc);
    return rc;
}

// void cclib::GPUParticleSort::sort_tail(int theCount, int theStep)
// {
//  doMergePass(theCount / 2, theStep * 2);
//
//  _myCurrentPass++;
//
//  if(!doNextPass()) {
//      printf(".");
// //           _myCurrentPass = 0;
//      return;
//  }
//
// //       _myCurrentPass++;
//
// //       printf("\tSORT current %10d, count %10d, step %10d\n",_myCurrentPass,theCount,theStep);
//
// //        if (DEBUG)
// //            CCLog.info(_myCurrentPass + ": mergeRec: count=" + theCount + ", step=" + theStep);
//
//  tail_shader(theCount, theStep);
// }

void cclib::GPUParticleSort::tail_shader(int theCount, int theStep)
{
//  printf("\ttail_shader, count %10d, step %10d\n", theCount, theStep);

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
}


// void cclib::GPUParticleSort::sort_head(int theCount, int theStep)
// {
//  _myCurrentPass++;
//
//  if(!doNextPass()) {
//      printf(".");
// //           _myCurrentPass = 0;
//      return;
//  }
//
// //       _myCurrentPass++;
//
// //       printf("\tEND current %10d, count %10d, step %10d\n",_myCurrentPass,theCount,theStep);
//
// //        if (DEBUG)
// //            CCLog.info(_myCurrentPass + ": mergeEnd: count="+theCount+", step="+theStep);
//
//  head_shader(theStep);
//
// }
//
void cclib::GPUParticleSort::head_shader(int theStep)
{
//  printf("\thead_shader, step %10d\n", theStep);
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

void cclib::GPUParticleSort::doMergePass(int theCount, int theStep)
{
    printf("\tdoMergePass current %10d, count %10d, step %10d\n", _myCurrentPass,theCount, theStep);

#if 0
    if (2 < theCount) {
        sort_tail(theCount, theStep);
    } else {
        sort_head(theCount, theStep);
    }
#else

    for(int c = 2, s = theStep; c < theCount; c <<= 1, s >>= 1) {
        printf("\tdoMergePass: count %10d, setp %d\n", c, s);
        _myCurrentPass++;
        if(doNextPass()) {
            tail_shader(c, s);
        } else {
            printf(".");
        }
    }

    _myCurrentPass++;
    if(doNextPass()) {
        head_shader(theStep);
    } else {
        printf(".");
    }

#endif
//    if(DEBUG){
//        //            FloatBuffer myData = _myBuffer.getData();
//        //            while(myData.hasRemaining()){
//        //                System.out.println(myData.get()+":"+myData.get()+":"+myData.get());
//        //            }
//    }
}
