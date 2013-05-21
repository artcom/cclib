//
//  gpubloom.h
//  Project
//
//  Created by Frank Eickhoff on 17.04.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#pragma once

#include <cclib.h>
#include <gl/framebufferobject.h>

#include "gl/glslshader.h"
#include "gl/shaderbuffer.h"
#include "gl/framebufferobject.h"
//#include "renderbuffer.h"
//#include "gpuseparategaussianblur.h"
//#include "ui_uitilities.h"

namespace cclib {
    
    class GPUBloom;
    typedef std::tr1::shared_ptr<GPUBloom> GPUBloomPtr;

    class GPUBloom
    {
//        cclib::RenderBufferPtr _myRenderTexture, _myRenderTexture1;
        cclib::FrameBufferObjectPtr _myRenderTexture, _myRenderTexture1;
        cclib::GLSLShaderPtr _myBloomShader;
//        cclib::GPUSeperateGaussianBlurPtr _myBlur;
        
////        @CCControl(name = "highlightRange", min = 0, max = 1)
//        float _cHighlightColor = cclib::ui::getValue("bloom", "highlightRange");
//        
////        @CCControl(name = "highlightScale", min = 0, max = 10)
//        float _cHighlightScale = cclib::ui::getValue("bloom", "highlightScale");
//        
////        @CCControl(name = "highlightPow", min = 0, max = 10)
//        float _cHighlightPow = cclib::ui::getValue("bloom", "highlightPow");
//        
////        @CCControl(name = "debug bloom")
////        bool _cDebugBloom = true;
//        
////        @CCControl(name = "apply bloom")
////        bool _cApplyBloom;
//        
////#define MAXIMUM_BLUR_RADIUS = 50;
//        
////        @CCControl(name = "blur radius", min = 0, max = MAXIMUM_BLUR_RADIUS)
////        float _cBlurRadius = cclib::ui::getValue("bloom", "blur radius")*2;
//        
////        @CCControl(name = "blur radius 2", min = 0, max = MAXIMUM_BLUR_RADIUS)
////        float _cBlurRadius2 = cclib::ui::getValue("bloom", "blur radius 2")*2;
        
        float _cHighlightColor, _cHighlightScale, _cHighlightPow;
        
        int _myWidth;
        int _myHeight;
        
    public:
        GPUBloom(int theWidth, int theHeight);
        
        static GPUBloomPtr create(int theWidth, int theHeight) {
            return cclib::GPUBloomPtr(new cclib::GPUBloom(theWidth, theHeight));
        };
        
        void start();
        void end();
    };
        
}; // namespace cclib
