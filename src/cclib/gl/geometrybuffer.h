//
//  GeometryBuffer.h
//  Project
//
//  Created by Frank Eickhoff on 23.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#ifndef __Project__GeometryBuffer__
#define __Project__GeometryBuffer__

#include <cclib.h>
#include <gl/framebufferobject.h>
#include <gl/glslshader.h>
#include <gl/graphics.h>

namespace cclib {
    
    class GeometryBuffer;
    typedef std::tr1::shared_ptr<GeometryBuffer> GeometryBufferPtr;
    
    class GeometryBuffer
    {
//        #define POSITIONS 0
//        #define NORMALS 1
//        #define COLORS 2
        
//        cclib::RenderBufferPtr _myRenderTexture; use framebuffer instead
        cclib::FrameBufferObjectPtr _myRenderTexture;
        cclib::GLSLShaderPtr _myShader;
        
//        private CCGraphics _myGraphics;
        
        int _myWidth;
        int _myHeight;
        
    public:
        
        GeometryBuffer(float theWidth, float theHeight);
        
        static GeometryBufferPtr create(float theWidth, float theHeight)
        {
            return GeometryBufferPtr(new GeometryBuffer(theWidth, theHeight));
        }
        
        void beginDraw();
        void updateMatrix();
        void updateMatrix(const std::vector<float>& matrix);
        void updateMatrix(const float* matrix);
        
//        CCMatrix4f inverseView(){
//            _myRenderTexture.camera().updateProjectionInfos();
//            return _myRenderTexture.camera().viewMatrix().invert();
//        }
    
        void endDraw();
    
//        public CCRenderBuffer data() {
//            return _myRenderTexture;
//        }
    
        int width() { return _myWidth; };
        int height() { return _myHeight; }; 

        cclib::Texture2DPtr positions()
        {
            return _myRenderTexture->attachment(0);
        }
        
        cclib::Texture2DPtr normals()
        {
            return _myRenderTexture->attachment(1);
        }
        
        cclib::Texture2DPtr colors()
        {
            return _myRenderTexture->attachment(2);
        }
        
        
    };
    
}; // namespace cclib

#endif
