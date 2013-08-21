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

#include <gl/glslshader.h>
#include <gl/shaderbuffer.h>
#include <gl/framebufferobject.h>

#define BLOOM_MAXIMUM_BLUR_RADIUS 50

namespace cclib {
    
class GPUBloom
{
    private:
        RenderBufferPtr _myRenderTexture, _myRenderTexture1;
        GLSLShaderPtr _myBloomShader;
        
        float _highlightColor;
        float _highlightScale;
        float _highlightPow; 
        bool _debugBloom;
        bool _applyBloom;

        float _blurRadius;
        float _blurRadius2;

        SeparateGaussianBlurPtr _myBlur;

        int _width;
        int _height;
        
        GPUBloom(int theWidth, int theHeight);
        
    public:
    
        static GPUBloomPtr create(int theWidth, int theHeight) {
            return cclib::GPUBloomPtr(new cclib::GPUBloom(theWidth, theHeight));
        };
        
        void start();
        void end();

        void endCapture();
        void applyBloom();
        void startBlur();
        void endBlur();

        void setHighlightColor(float highlightColor) {
            _highlightColor = highlightColor;
        };
    
        void setHighlightScale(float highlightScale) {
            _highlightScale = highlightScale;
        };

        void setHighlightPow(float highlightPow) {
            _highlightPow = highlightPow;
        };
    
        void setDebugBloom(bool debugBloom) {
            _debugBloom = debugBloom;
        };
    
        void setApplyBloom(bool applyBloom) {
            _applyBloom = applyBloom;
        };
    
        void setBlurRadius(int radius1, int radius2 = -1) {
            _blurRadius = radius1;
            if (radius2 == -1) {
                _blurRadius2 = radius1;
            } else {
                _blurRadius2 = radius2;
            }
        };
};
    
}; // namespace cclib
