//
//  gpuparticlesort.h
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#pragma once

#include <cclib.h>

namespace cclib {

    class GPUParticleSort
    {

    public:
//        GPUParticleSort();
        GPUParticleSort(cclib::GPUParticles* theParticles);

//        static GPUParticleSortPtr create() {
//            return cclib::GPUParticleSortPtr(new cclib::GPUParticleSort());
//        };

        static GPUParticleSortPtr create(cclib::GPUParticles* theParticles) {
            return cclib::GPUParticleSortPtr(new cclib::GPUParticleSort(theParticles));
        };

        void reset();
        void update(float theDeltaTime);

		cclib::ShaderBufferPtr indices() { return _myBuffer; };
// 		cclib::ShaderBufferPtr distances() { return _myDestinationBuffer; };

	private:
        bool doNextPass();

        void mergeSort();
        void doMergePass(int theCount, int theStep);

        void doMergeSortPass(int theCount);

        cclib::ShaderBufferPtr _myBuffer;
        cclib::ShaderBufferPtr _myDestinationBuffer;

        int _myCurrentPass;
        int _myBeginPass;
        int _myEndPass;
        int _mySortPassesPerFrame;
        int _myMaxSortPasses;

        cclib::CGShaderPtr _mySortRecursionShader;
        cclib::CGShaderPtr _mySortEndShader;
        CGparameter _mySortRecursionShaderSizeParameter;
        CGparameter _mySortRecursionShaderSortStepParameter;
        CGparameter _mySortRecursionShaderSortCountParameter;
        CGparameter _mySortEndShaderSizeParameter;
        CGparameter _mySortEndShaderSortStepParameter;

		cclib::CGShaderPtr _myDistanceSortInitShader;
		cclib::CGShaderPtr _myComputeDistanceShader;
		cclib::CGShaderPtr _myLookupPositionPositionShader;
//        CGparameter DistanceSortCompute_ViewerPosition_Param;

        cclib::GPUParticlesPtr _myParticles;

		void sort_tail(int theCount, int theStep);
		void sort_head(int theCount, int theStep);
		void tail_shader(int theCount, int theStep);
		void head_shader(int theStep);

    };

}; // namespace cclib
