//
//  _matrix.h
//  Project
//
//  Created by Frank Eickhoff on 27.05.13.
//  Copyright (c) 2013 Project. All rights reserved.
//

#ifndef __Project__Matrix__
#define __Project__Matrix__

#include <cclib.h>

namespace cclib {
    
    template<typename T>
    class Matrix4
    {
    private:

        float _m00, _m01, _m02, _m03;
        float _m10, _m11, _m12, _m13;
        float _m20, _m21, _m22, _m23;
        float _m30, _m31, _m32, _m33;
        
//        static Matrix4<T>ZERO = new CCMatrix4f(
//                                             0, 0, 0, 0,
//                                             0, 0, 0, 0,
//                                             0, 0, 0, 0,
//                                             0, 0, 0, 0
//                                             );
//        static Matrix4<T>IDENTITY = new CCMatrix4f();
        
#define DEFAULT_STACK_DEPTH 0
        int _maxStackDepth;
        int _stackPointer;
//        float stack[][];
        
        // locally allocated version to avoid creating new memory
//        static Matrix4<T>inverseCopy;
        
    public:
        
        Matrix4(): _maxStackDepth(DEFAULT_STACK_DEPTH), _stackPointer(0)
        {
            loadIdentity();
        }
        
        ~Matrix4() {};
        
//        CCMatrix4f(int stackDepth) {
//            set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//            stack = new float[stackDepth][16];
//            maxStackDepth = stackDepth;
//        }
//        
//        CCMatrix4f(CCVector3f theV0, CCVector3f theV1, CCVector3f theV2){
//            m00 = theV0.x;
//            m01 = theV0.y;
//            m02 = theV0.z;
//            m03 = 0.0f;
//            
//            m10 = theV1.x;
//            m11 = theV1.y;
//            m12 = theV1.z;
//            m13 = 0.0f;
//            
//            m20 = theV2.x;
//            m21 = theV2.y;
//            m22 = theV2.z;
//            m23 = 0.0f;
//            
//            m30 = 0.0f;
//            m31 = 0.0f;
//            m32 = 0.0f;
//            m33 = 1.0f;
//        }
//        
//        CCMatrix4f(
//                          float m00, float m01, float m02, float m03,
//                          float m10, float m11, float m12, float m13,
//                          float m20, float m21, float m22, float m23,
//                          float m30, float m31, float m32, float m33
//                          ) {
//            set(
//                m00, m01, m02, m03,
//                m10, m11, m12, m13,
//                m20, m21, m22, m23,
//                m30, m31, m32, m33
//                );
//            maxStackDepth = DEFAULT_STACK_DEPTH;
//        }
//        
//        /**
//         * Copy constructor that creates a new <code>CCMatrix4f</code> object that
//         * is the same as the provided matrix.
//         *
//         * @param theMatrix the matrix to copy.
//         */
//        CCMatrix4f(Matrix4<T>src) {
//            set(src);
//            maxStackDepth = src.maxStackDepth;
//            stack = new float[maxStackDepth][16];
//        }
//
        
        /**
         * <code>loadIdentity</code> sets this matrix to the identity matrix.
         * Where all values are zero except those along the diagonal which are one.
         */
        void loadIdentity() {
            set(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
                );
        }

        void loadModelView()
        {
            GLfloat m[16];
            glGetFloatv (GL_MODELVIEW_MATRIX, m);
            
            set(
                m[0], m[4], m[8], m[12],
                m[1], m[5], m[9], m[13],
                m[2], m[6], m[10], m[14],
                m[3], m[7], m[11], m[15]
                );

        }
        
//        void reset() {
//            set(
//                1, 0, 0, 0,
//                0, 1, 0, 0,
//                0, 0, 1, 0,
//                0, 0, 0, 1
//                );
//        }
//        
//        void clearStack() {
//            stackPointer = 0;
//        }
//        
//        boolean push() {
//            if (stackPointer == maxStackDepth)
//                return false;
//            
//            stack[stackPointer][0] = m00;
//            stack[stackPointer][1] = m01;
//            stack[stackPointer][2] = m02;
//            stack[stackPointer][3] = m03;
//            
//            stack[stackPointer][4] = m10;
//            stack[stackPointer][5] = m11;
//            stack[stackPointer][6] = m12;
//            stack[stackPointer][7] = m13;
//            
//            stack[stackPointer][8] = m20;
//            stack[stackPointer][9] = m21;
//            stack[stackPointer][10] = m22;
//            stack[stackPointer][11] = m23;
//            
//            stack[stackPointer][12] = m30;
//            stack[stackPointer][13] = m31;
//            stack[stackPointer][14] = m32;
//            stack[stackPointer][15] = m33;
//            
//            stackPointer++;
//            return true;
//        }
//        
//        boolean pop() {
//            if (stackPointer == 0)
//                return false;
//            stackPointer--;
//            
//            m00 = stack[stackPointer][0];
//            m01 = stack[stackPointer][1];
//            m02 = stack[stackPointer][2];
//            m03 = stack[stackPointer][3];
//            
//            m10 = stack[stackPointer][4];
//            m11 = stack[stackPointer][5];
//            m12 = stack[stackPointer][6];
//            m13 = stack[stackPointer][7];
//            
//            m20 = stack[stackPointer][8];
//            m21 = stack[stackPointer][9];
//            m22 = stack[stackPointer][10];
//            m23 = stack[stackPointer][11];
//            
//            m30 = stack[stackPointer][12];
//            m31 = stack[stackPointer][13];
//            m32 = stack[stackPointer][14];
//            m33 = stack[stackPointer][15];
//            
//            return true;
//        }
//        
//        void set(Matrix4<T>src) {
//            set(
//                src.m00, src.m01, src.m02, src.m03,
//                src.m10, src.m11, src.m12, src.m13,
//                src.m20, src.m21, src.m22, src.m23,
//                src.m30, src.m31, src.m32, src.m33
//                );
//        }
//        
//        Matrix4<T>set(CCMatrix3f src) {
//            set(
//                src.m00, src.m01, src.m02, 0,
//                src.m10, src.m11, src.m12, 0,
//                src.m20, src.m21, src.m22, 0,
//                0, 0, 0, 1
//                );
//            return this;
//        }
        
        void set(
                float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33
                ) {
            _m00 = m00; _m01 = m01; _m02 = m02; _m03 = m03;
            _m10 = m10; _m11 = m11; _m12 = m12; _m13 = m13;
            _m20 = m20; _m21 = m21; _m22 = m22; _m23 = m23;
            _m30 = m30; _m31 = m31; _m32 = m32; _m33 = m33;
        }
//
//        Matrix4<T>setFromColumOrder(float...theGLMatrix){
//            if(theGLMatrix.length != 16)return this;
//            m00 = theGLMatrix[0]; m01 = theGLMatrix[4]; m02 = theGLMatrix[8]; m03 = theGLMatrix[12];
//            m10 = theGLMatrix[1]; m11 = theGLMatrix[5]; m12 = theGLMatrix[9]; m13 = theGLMatrix[13];
//            m20 = theGLMatrix[2]; m21 = theGLMatrix[6]; m22 = theGLMatrix[10]; m23 = theGLMatrix[14];
//            m30 = theGLMatrix[3]; m31 = theGLMatrix[7]; m32 = theGLMatrix[11]; m33 = theGLMatrix[15];
//            return this;
//        }
//        
//        /**
//         * Sets this matrix by an array in row order
//         * @param theRowOrderMatrix
//         */
//        Matrix4<T>setFromRowOrder(float...theRowOrderMatrix) {
//            if(theRowOrderMatrix.length != 16)return this;
//            m00 = theRowOrderMatrix[0]; m01 = theRowOrderMatrix[1]; m02 = theRowOrderMatrix[2]; m03 = theRowOrderMatrix[3];
//            m10 = theRowOrderMatrix[4]; m11 = theRowOrderMatrix[5]; m12 = theRowOrderMatrix[6]; m13 = theRowOrderMatrix[7];
//            m20 = theRowOrderMatrix[8]; m21 = theRowOrderMatrix[9]; m22 = theRowOrderMatrix[10]; m23 = theRowOrderMatrix[11];
//            m30 = theRowOrderMatrix[12]; m31 = theRowOrderMatrix[13]; m32 = theRowOrderMatrix[14]; m33 = theRowOrderMatrix[15];
//            return this;
//        }
//        
//        void translate(float tx, float ty) {
//            translate(tx, ty, 0);
//        }
//        
//        void invTranslate(float tx, float ty) {
//            invTranslate(tx, ty, 0);
//        }
//        
//        Matrix4<T>translate(float theX, float theY, float theZ) {
//            m03 += theX * m00 + theY * m01 + theZ * m02;
//            m13 += theX * m10 + theY * m11 + theZ * m12;
//            m23 += theX * m20 + theY * m21 + theZ * m22;
//            m33 += theX * m30 + theY * m31 + theZ * m32;
//            
//            return this;
//        }
//        
//        Matrix4<T>translate(CCVector3f theVector){
//            return translate(theVector.x, theVector.y, theVector.z);
//        }
//        
//        Matrix4<T>translate(CCVector2f theVector){
//            return translate(theVector.x, theVector.y, 0);
//        }
//        
//        void invTranslate(float tx, float ty, float tz) {
//            preApply(1, 0, 0, -tx, 0, 1, 0, -ty, 0, 0, 1, -tz, 0, 0, 0, 1);
//        }
//        
//        void shere(float theX, float theY){
//            apply(
//                  1,    0,    0, 0,
//                  0,    1,    0, 0,
//                  theX, theY, 1, 0,
//                  0,    0,    0, 1
//                  );
//        }
//        
//        // OPT could save several multiplies for the 0s and 1s by just
//        //     putting the multMatrix code here and removing uneccessary terms
//        
//        private void rotateX(float s, float c) {
//            apply(
//                  1, 0, 0, 0,
//                  0, c, -s, 0,
//                  0, s, c, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void rotateX(float theAngle) {
//            float c = cos(theAngle);
//            float s = sin(theAngle);
//            rotateX(s,c);
//        }
//        
//        void invRotateX(float theAngle) {
//            float c = cos(-theAngle);
//            float s = sin(-theAngle);
//            preApply(1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1);
//        }
//        
//        private void rotateY(float s, float c) {
//            apply(
//                  c, 0, s, 0,
//                  0, 1, 0, 0,
//                  -s, 0, c, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void rotateY(float theAngle) {
//            float c = cos(theAngle);
//            float s = sin(theAngle);
//            rotateY(s,c);
//        }
//        
//        void invRotateY(float theAngle) {
//            float c = cos(-theAngle);
//            float s = sin(-theAngle);
//            preApply(c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1);
//        }
//        
//        /**
//         * Just calls rotateZ because two dimensional rotation
//         * is the same as rotating along the z-axis.
//         */
//        void rotate(float theAngle) {
//            rotateZ(theAngle);
//        }
//        
//        void invRotate(float theAngle) {
//            invRotateZ(theAngle);
//        }
//        
//        void rotateZ(float theAngle) {
//            float c = cos(theAngle);
//            float s = sin(theAngle);
//            apply(
//                  c, -s, 0, 0,
//                  s, c, 0, 0,
//                  0, 0, 1, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void invRotateZ(float theAngle) {
//            float c = cos(-theAngle);
//            float s = sin(-theAngle);
//            preApply(c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//        }
//        
//        void rotate(CCQuaternion theQuaternion) {
//            CCVector4f myRotation = theQuaternion.getVectorAndAngle();
//            rotate(CCMath.degrees(myRotation.w()), myRotation.x, myRotation.y, myRotation.z);
//        }
//        
//        void rotate(float theAngle, float v0, float v1, float v2) {
//            // should be in radians (i think), instead of degrees (gl uses degrees)
//            // based on 15-463 code, but similar to opengl ref p.443
//            
//            // TODO should make sure this vector is normalized
//            
//            float c = cos(theAngle);
//            float s = sin(theAngle);
//            float t = 1.0f - c;
//            
//            apply(
//                  (t * v0 * v0) + c		, (t * v0 * v1) - (s * v2), (t * v0 * v2) + (s * v1), 0,
//                  (t * v0 * v1) + (s * v2), (t * v1 * v1) + c		  , (t * v1 * v2) - (s * v0), 0,
//                  (t * v0 * v2) - (s * v1), (t * v1 * v2) + (s * v0), (t * v2 * v2) + c       , 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void rotate(float theAngle, CCVector3f theVector) {
//            rotate(theAngle, theVector.x, theVector.y, theVector.z);
//        }
//        
//        void invRotate(float theAngle, float v0, float v1, float v2) {
//            // TODO should make sure this vector is normalized
//            
//            float c = cos(-theAngle);
//            float s = sin(-theAngle);
//            float t = 1.0f - c;
//            
//            preApply(
//                     (t * v0 * v0) + c, (t * v0 * v1) - (s * v2), (t * v0 * v2) + (s * v1), 0,
//                     (t * v0 * v1) + (s * v2), (t * v1 * v1) + c, (t * v1 * v2) - (s * v0), 0,
//                     (t * v0 * v2) - (s * v1), (t * v1 * v2) + (s * v0), (t * v2 * v2) + c, 0,
//                     0, 0, 0, 1
//                     );
//        }
//        
//        void scale(float s) {
//            apply(
//                  s, 0, 0, 0,
//                  0, s, 0, 0,
//                  0, 0, s, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void invScale(float s) {
//            preApply(
//                     1 / s, 0, 0, 0,
//                     0, 1 / s, 0, 0,
//                     0, 0, 1 / s, 0,
//                     0, 0, 0, 1
//                     );
//        }
//        
//        void scale(float sx, float sy) {
//            apply(
//                  sx, 0, 0, 0,
//                  0, sy, 0, 0,
//                  0, 0, 1, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void invScale(float sx, float sy) {
//            preApply(1 / sx, 0, 0, 0, 0, 1 / sy, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//        }
//        
//        // OPTIMIZE: same as above
//        void scale(float x, float y, float z) {
//            apply(
//                  x, 0, 0, 0,
//                  0, y, 0, 0,
//                  0, 0, z, 0,
//                  0, 0, 0, 1
//                  );
//        }
//        
//        void invScale(float x, float y, float z) {
//            preApply(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
//        }
//        
//        void lookAt (CCVector3f theEye, CCVector3f theCenter, CCVector3f theUp) {
//            
//            CCVector3f myZ = CCVecMath.subtract(theEye, theCenter).normalize();
//            
//            if ( myZ.lengthSquared() == 0) {
//                myZ.z = 1;
//            }
//            
//            CCVector3f myX = CCVecMath.cross(theUp, myZ).normalize();
//            
//            if ( myX.lengthSquared() == 0 ) {
//                myZ.x += 0.0001;
//                myX = CCVecMath.cross(theUp, myZ).normalize();
//            }
//            
//            CCVector3f myY = CCVecMath.cross(myZ, myX).normalize();
//            
//            m00 = myX.x; m10 = myY.x; m20 = myZ.x;
//            m01 = myX.y; m11 = myY.y; m21 = myZ.y;
//            m02 = myX.z; m12 = myY.z; m22 = myZ.z;
//        }
//        
//        void preApply(Matrix4<T>lhs) {
//            preApply(
//                     lhs.m00, lhs.m01, lhs.m02, lhs.m03,
//                     lhs.m10, lhs.m11, lhs.m12, lhs.m13,
//                     lhs.m20, lhs.m21, lhs.m22, lhs.m23,
//                     lhs.m30, lhs.m31, lhs.m32, lhs.m33
//                     );
//        }
//        
//        // for inverse operations, like multiplying the matrix on the left
//        Matrix4<T>preApply(
//                                   float n00, float n01, float n02, float n03,
//                                   float n10, float n11, float n12, float n13,
//                                   float n20, float n21, float n22, float n23,
//                                   float n30, float n31, float n32, float n33
//                                   ) {
//            
//            float r00 = n00 * m00 + n01 * m10 + n02 * m20 + n03 * m30;
//            float r01 = n00 * m01 + n01 * m11 + n02 * m21 + n03 * m31;
//            float r02 = n00 * m02 + n01 * m12 + n02 * m22 + n03 * m32;
//            float r03 = n00 * m03 + n01 * m13 + n02 * m23 + n03 * m33;
//            
//            float r10 = n10 * m00 + n11 * m10 + n12 * m20 + n13 * m30;
//            float r11 = n10 * m01 + n11 * m11 + n12 * m21 + n13 * m31;
//            float r12 = n10 * m02 + n11 * m12 + n12 * m22 + n13 * m32;
//            float r13 = n10 * m03 + n11 * m13 + n12 * m23 + n13 * m33;
//            
//            float r20 = n20 * m00 + n21 * m10 + n22 * m20 + n23 * m30;
//            float r21 = n20 * m01 + n21 * m11 + n22 * m21 + n23 * m31;
//            float r22 = n20 * m02 + n21 * m12 + n22 * m22 + n23 * m32;
//            float r23 = n20 * m03 + n21 * m13 + n22 * m23 + n23 * m33;
//            
//            float r30 = n30 * m00 + n31 * m10 + n32 * m20 + n33 * m30;
//            float r31 = n30 * m01 + n31 * m11 + n32 * m21 + n33 * m31;
//            float r32 = n30 * m02 + n31 * m12 + n32 * m22 + n33 * m32;
//            float r33 = n30 * m03 + n31 * m13 + n32 * m23 + n33 * m33;
//            
//            m00 = r00;
//            m01 = r01;
//            m02 = r02;
//            m03 = r03;
//            m10 = r10;
//            m11 = r11;
//            m12 = r12;
//            m13 = r13;
//            m20 = r20;
//            m21 = r21;
//            m22 = r22;
//            m23 = r23;
//            m30 = r30;
//            m31 = r31;
//            m32 = r32;
//            m33 = r33;
//            
//            return this;
//        }
//        
//        boolean invApply(Matrix4<T>rhs) {
//            Matrix4<T>copy = new CCMatrix4f(rhs);
//            Matrix4<T>inverse = copy.invert();
//            if (inverse == null)
//                return false;
//            preApply(inverse);
//            return true;
//        }
//        
//        boolean invApply(
//                                float n00, float n01, float n02, float n03,
//                                float n10, float n11, float n12, float n13,
//                                float n20, float n21, float n22, float n23,
//                                float n30, float n31, float n32, float n33
//                                ) {
//            if (inverseCopy == null) {
//                inverseCopy = new CCMatrix4f();
//            }
//            inverseCopy.set(
//                            n00, n01, n02, n03,
//                            n10, n11, n12, n13,
//                            n20, n21, n22, n23,
//                            n30, n31, n32, n33
//                            );
//            
//            Matrix4<T>inverse = inverseCopy.invert();
//            if (inverse == null)
//                return false;
//            preApply(inverse);
//            return true;
//        }
//        
//        /**
//         * Applies the
//         * @param theMatrix
//         */
//        Matrix4<T>apply(Matrix4<T>theMatrix) {
//            return apply(
//                         theMatrix.m00, theMatrix.m01, theMatrix.m02, theMatrix.m03,
//                         theMatrix.m10, theMatrix.m11, theMatrix.m12, theMatrix.m13,
//                         theMatrix.m20, theMatrix.m21, theMatrix.m22, theMatrix.m23,
//                         theMatrix.m30, theMatrix.m31, theMatrix.m32, theMatrix.m33
//                         );
//        }
//        
//        Matrix4<T>apply(
//                                float n00, float n01, float n02, float n03,
//                                float n10, float n11, float n12, float n13,
//                                float n20, float n21, float n22, float n23,
//                                float n30, float n31, float n32, float n33
//                                ) {
//            
//            float r00 = m00 * n00 + m01 * n10 + m02 * n20 + m03 * n30;
//            float r01 = m00 * n01 + m01 * n11 + m02 * n21 + m03 * n31;
//            float r02 = m00 * n02 + m01 * n12 + m02 * n22 + m03 * n32;
//            float r03 = m00 * n03 + m01 * n13 + m02 * n23 + m03 * n33;
//            
//            float r10 = m10 * n00 + m11 * n10 + m12 * n20 + m13 * n30;
//            float r11 = m10 * n01 + m11 * n11 + m12 * n21 + m13 * n31;
//            float r12 = m10 * n02 + m11 * n12 + m12 * n22 + m13 * n32;
//            float r13 = m10 * n03 + m11 * n13 + m12 * n23 + m13 * n33;
//            
//            float r20 = m20 * n00 + m21 * n10 + m22 * n20 + m23 * n30;
//            float r21 = m20 * n01 + m21 * n11 + m22 * n21 + m23 * n31;
//            float r22 = m20 * n02 + m21 * n12 + m22 * n22 + m23 * n32;
//            float r23 = m20 * n03 + m21 * n13 + m22 * n23 + m23 * n33;
//            
//            float r30 = m30 * n00 + m31 * n10 + m32 * n20 + m33 * n30;
//            float r31 = m30 * n01 + m31 * n11 + m32 * n21 + m33 * n31;
//            float r32 = m30 * n02 + m31 * n12 + m32 * n22 + m33 * n32;
//            float r33 = m30 * n03 + m31 * n13 + m32 * n23 + m33 * n33;
//            
//            m00 = r00;
//            m01 = r01;
//            m02 = r02;
//            m03 = r03;
//            m10 = r10;
//            m11 = r11;
//            m12 = r12;
//            m13 = r13;
//            m20 = r20;
//            m21 = r21;
//            m22 = r22;
//            m23 = r23;
//            m30 = r30;
//            m31 = r31;
//            m32 = r32;
//            m33 = r33;
//            
//            return this;
//        }
//        
//        CCVector3f transform(CCVector3f theInput, CCVector3f theOutput) {
//            
//            if(theOutput == null)theOutput = new CCVector3f();
//            // must use these temp vars because vec may be the same as out
//            float tmpx = m00 * theInput.x + m10 * theInput.y + m20 * theInput.z + m03;
//            float tmpy = m01 * theInput.x + m11 * theInput.y + m21 * theInput.z + m13;
//            float tmpz = m02 * theInput.x + m12 * theInput.y + m22 * theInput.z + m23;
//            
//            theOutput.set(tmpx, tmpy, tmpz);
//            
//            return theOutput;
//        }
//        
//        CCVector4f transform(CCVector4f theVector) {
//            return transform(theVector,null);
//        }
//        
//        /**
//         * @param theAxis
//         */
//        CCVector3f rotateAxis(CCVector3f theAxis) {
//            transform(theAxis, theAxis);
//            theAxis.normalize();
//            return theAxis;
//        }
//        
//        void setRotationFromMatrix (CCVector3f theVector) {
//            float cosY = CCMath.cos(theVector.y);
//            
//            theVector.y = CCMath.asin(m02);
//            
//            if ( Math.abs( cosY ) > 0.00001 ) {
//                theVector.x = CCMath.atan2( - m12 / cosY, m22 / cosY );
//                theVector.z = CCMath.atan2( - m01 / cosY, m00 / cosY );
//            } else {
//                theVector.x = 0;
//                theVector.z = CCMath.atan2(m10, m11 );
//            }
//        }
//        
//        CCVector4f transform(CCVector4f theInput, CCVector4f theOutput) {
//            
//            if(theOutput == null)theOutput = new CCVector4f();
//            // must use these temp vars because vec may be the same as out
//            float tmpx = m00 * theInput.x + m01 * theInput.y + m02 * theInput.z + m03 * theInput.w;
//            float tmpy = m10 * theInput.x + m11 * theInput.y + m12 * theInput.z + m13 * theInput.w;
//            float tmpz = m20 * theInput.x + m21 * theInput.y + m22 * theInput.z + m23 * theInput.w;
//            float tmpw = m30 * theInput.x + m31 * theInput.y + m32 * theInput.z + m33 * theInput.w;
//            
//            theOutput.set(tmpx, tmpy, tmpz,tmpw);
//            
//            return theOutput;
//        }
//        
//        CCVector3f transform(CCVector3f theVector) {
//            return transform(theVector,theVector);
//        }
//        
//        void inverseTransform(CCVector3f theInput, CCVector3f theOutput) {
//            float inx = theInput.x  - m03;
//            float iny = theInput.y  - m13;
//            float inz = theInput.z  - m23;
//            float tmpx = m00 * inx + m10 * iny + m20 * inz;
//            float tmpy = m01 * inx + m11 * iny + m21 * inz;
//            float tmpz = m02 * inx + m12 * iny + m22 * inz;
//            //		System.out.println(m30+":"+m31+":"+m32+"   "+m03+":"+m13+":"+m23);
//            theOutput.set(tmpx, tmpy, tmpz);
//        }
//        
//        CCVector3f inverseTransform(CCVector3f theVector) {
//            inverseTransform(theVector,theVector);
//            return theVector;
//        }
        
        /**
         * @return the determinant of the matrix
         */
        float determinant() {
            float f;
            f  = _m00 * ((_m11 * _m22 * _m33 + _m12 * _m23 * _m31 + _m13 * _m21 * _m32) - _m13 * _m22 * _m31 - _m11 * _m23 * _m32 - _m12 * _m21 * _m33);
            f -= _m01 * ((_m10 * _m22 * _m33 + _m12 * _m23 * _m30 + _m13 * _m20 * _m32) - _m13 * _m22 * _m30 - _m10 * _m23 * _m32 - _m12 * _m20 * _m33);
            f += _m02 * ((_m10 * _m21 * _m33 + _m11 * _m23 * _m30 + _m13 * _m20 * _m31) - _m13 * _m21 * _m30 - _m10 * _m23 * _m31 - _m11 * _m20 * _m33);
            f -= _m03 * ((_m10 * _m21 * _m32 + _m11 * _m22 * _m30 + _m12 * _m20 * _m31) - _m12 * _m21 * _m30 - _m10 * _m22 * _m31 - _m11 * _m20 * _m32);
            return f;
        }

        /**
         * Calculate the determinant of a 3x3 matrix
         * @return result
         */
        float determinant3x3(
                                     float t00, float t01, float t02,
                                     float t10, float t11, float t12,
                                     float t20, float t21, float t22
                                     )
        {
            return
                    t00 * (t11 * t22 - t12 * t21) +
                    t01 * (t12 * t20 - t10 * t22) +
                    t02 * (t10 * t21 - t11 * t20);
        }
        
//        Matrix4<T>transpose() {
//            float temp;
//            temp = m01; m01 = m10; m10 = temp;
//            temp = m02; m02 = m20; m20 = temp;
//            temp = m03; m03 = m30; m30 = temp;
//            temp = m12; m12 = m21; m21 = temp;
//            temp = m13; m13 = m31; m31 = temp;
//            temp = m23; m23 = m32; m32 = temp;
//            return this;
//        }
        
        /**
         * Invert this matrix
         * @return this if successful, null otherwise
         */
        bool invert()
        {
            
            float determinant_ = determinant();
            
            if (determinant_ != 0) {
                // m00 m01 m02 m03
                // m10 m11 m12 m13
                // m20 m21 m22 m23
                // m30 m31 m32 m33
                float determinant_inv = 1.f / determinant_;
                
                // first row
                float t00 = determinant3x3(_m11, _m12, _m13, _m21, _m22, _m23, _m31, _m32, _m33);
                float t01 = -determinant3x3(_m10, _m12, _m13, _m20, _m22, _m23, _m30, _m32, _m33);
                float t02 = determinant3x3(_m10, _m11, _m13, _m20, _m21, _m23, _m30, _m31, _m33);
                float t03 = -determinant3x3(_m10, _m11, _m12, _m20, _m21, _m22, _m30, _m31, _m32);
                
                // second row
                float t10 = -determinant3x3(_m01, _m02, _m03, _m21, _m22, _m23, _m31, _m32, _m33);
                float t11 = determinant3x3(_m00, _m02, _m03, _m20, _m22, _m23, _m30, _m32, _m33);
                float t12 = -determinant3x3(_m00, _m01, _m03, _m20, _m21, _m23, _m30, _m31, _m33);
                float t13 = determinant3x3(_m00, _m01, _m02, _m20, _m21, _m22, _m30, _m31, _m32);
                
                // third row
                float t20 = determinant3x3(_m01, _m02, _m03, _m11, _m12, _m13, _m31, _m32, _m33);
                float t21 = -determinant3x3(_m00, _m02, _m03, _m10, _m12, _m13, _m30, _m32, _m33);
                float t22 = determinant3x3(_m00, _m01, _m03, _m10, _m11, _m13, _m30, _m31, _m33);
                float t23 = -determinant3x3(_m00, _m01, _m02, _m10, _m11, _m12, _m30, _m31, _m32);
                
                // fourth row
                float t30 = -determinant3x3(_m01, _m02, _m03, _m11, _m12, _m13, _m21, _m22, _m23);
                float t31 = determinant3x3(_m00, _m02, _m03, _m10, _m12, _m13, _m20, _m22, _m23);
                float t32 = -determinant3x3(_m00, _m01, _m03, _m10, _m11, _m13, _m20, _m21, _m23);
                float t33 = determinant3x3(_m00, _m01, _m02, _m10, _m11, _m12, _m20, _m21, _m22);
                
                // transpose and divide by the determinant
                _m00 = t00 * determinant_inv;
                _m11 = t11 * determinant_inv;
                _m22 = t22 * determinant_inv;
                _m33 = t33 * determinant_inv;
                _m01 = t10 * determinant_inv;
                _m10 = t01 * determinant_inv;
                _m20 = t02 * determinant_inv;
                _m02 = t20 * determinant_inv;
                _m12 = t21 * determinant_inv;
                _m21 = t12 * determinant_inv;
                _m03 = t30 * determinant_inv;
                _m30 = t03 * determinant_inv;
                _m13 = t31 * determinant_inv;
                _m31 = t13 * determinant_inv;
                _m32 = t23 * determinant_inv;
                _m23 = t32 * determinant_inv;

                return true;
            }
            return false;
        }
        
//        //////////////////////////////////////////////////////////////
//        
//        String toString() {
//            int big = (int) maxabs(
//                                   m00, m01, m02, m03,
//                                   m10, m11, m12, m13,
//                                   m20, m21, m22, m23,
//                                   m30, m31, m32, m33
//                                   );
//            
//            // avoid infinite loop
//            if (Float.isNaN(big) || Float.isInfinite(big)) {
//                big = 1000000; // set to something arbitrary
//            }
//            
//            int d = 1;
//            while ((big /= 10) != 0)
//                d++; // cheap log()
//            
//            StringBuilder myStringBuilder = new StringBuilder();
//            myStringBuilder.append(
//                                   CCFormatUtil.nfs(m00, d, 4) + " " +
//                                   CCFormatUtil.nfs(m01, d, 4) + " " +
//                                   CCFormatUtil.nfs(m02, d, 4) + " " +
//                                   CCFormatUtil.nfs(m03, d, 4) + "\n");
//            
//            myStringBuilder.append(
//                                   CCFormatUtil.nfs(m10, d, 4) + " " +
//                                   CCFormatUtil.nfs(m11, d, 4) + " " +
//                                   CCFormatUtil.nfs(m12, d, 4) + " " +
//                                   CCFormatUtil.nfs(m13, d, 4) + "\n");
//            
//            myStringBuilder.append(
//                                   CCFormatUtil.nfs(m20, d, 4) + " " +
//                                   CCFormatUtil.nfs(m21, d, 4) + " " +
//                                   CCFormatUtil.nfs(m22, d, 4) + " " +
//                                   CCFormatUtil.nfs(m23, d, 4) + "\n");
//            
//            myStringBuilder.append(
//                                   CCFormatUtil.nfs(m30, d, 4) + " " +
//                                   CCFormatUtil.nfs(m31, d, 4) + " " +
//                                   CCFormatUtil.nfs(m32, d, 4) + " " +
//                                   CCFormatUtil.nfs(m33, d, 4) + "\n");
//            
//            return myStringBuilder.toString();
//        }
//        
//        Matrix4<T>clone() {
//            return new CCMatrix4f(this);
//        }
//        
//        //////////////////////////////////////////////////////////////
//        
//        private float maxabs(float ... theValue) {
//            float result = Float.MIN_VALUE;
//            for(float myValue:theValue) {
//                result = CCMath.max(CCMath.abs(myValue), result);
//            }
//            return result;
//        }
//        
//        private static float sin(float theAngle) {
//            return (float) Math.sin(theAngle);
//        }
//        
//        private static float cos(float theAngle) {
//            return (float) Math.cos(theAngle);
//        }
//        
//        FloatBuffer toFloatBuffer() {
//            FloatBuffer myResult = FloatBuffer.allocate(16);
//            
//            myResult.put(m00);myResult.put(m10);myResult.put(m20);myResult.put(m30);
//            myResult.put(m01);myResult.put(m11);myResult.put(m21);myResult.put(m31);
//            myResult.put(m02);myResult.put(m12);myResult.put(m22);myResult.put(m32);
//            myResult.put(m03);myResult.put(m13);myResult.put(m23);myResult.put(m33);
//            myResult.rewind();
//            return myResult;
//        }
        
        std::vector<float> toFloatBuffer()
        {
            std::vector<float> array;
            
            array.push_back(_m00);array.push_back(_m10);array.push_back(_m20);array.push_back(_m30);
            array.push_back(_m01);array.push_back(_m11);array.push_back(_m21);array.push_back(_m31);
            array.push_back(_m02);array.push_back(_m12);array.push_back(_m22);array.push_back(_m32);
            array.push_back(_m03);array.push_back(_m13);array.push_back(_m23);array.push_back(_m33);
            
            return array;
        }
        
//        static Matrix4<T>createFromGLMatrix(FloatBuffer theGLMatrix){
//            theGLMatrix.rewind();
//            Matrix4<T>myResult = new CCMatrix4f();
//            myResult.m00 = theGLMatrix.get();myResult.m10 = theGLMatrix.get();myResult.m20 = theGLMatrix.get();myResult.m30 = theGLMatrix.get();
//            myResult.m01 = theGLMatrix.get();myResult.m11 = theGLMatrix.get();myResult.m21 = theGLMatrix.get();myResult.m31 = theGLMatrix.get();
//            myResult.m02 = theGLMatrix.get();myResult.m12 = theGLMatrix.get();myResult.m22 = theGLMatrix.get();myResult.m32 = theGLMatrix.get();
//            myResult.m03 = theGLMatrix.get();myResult.m13 = theGLMatrix.get();myResult.m23 = theGLMatrix.get();myResult.m33 = theGLMatrix.get();
//            return myResult;
//        }
//        
//        static Matrix4<T>createFromGLMatrix(double[] theGLMatrix){
//            Matrix4<T>myResult = new CCMatrix4f();
//            myResult.m00 = (float)theGLMatrix[ 0];myResult.m10 = (float)theGLMatrix[ 1];myResult.m20 = (float)theGLMatrix[ 2];myResult.m30 = (float)theGLMatrix[ 3];
//            myResult.m01 = (float)theGLMatrix[ 4];myResult.m11 = (float)theGLMatrix[ 5];myResult.m21 = (float)theGLMatrix[ 6];myResult.m31 = (float)theGLMatrix[ 7];
//            myResult.m02 = (float)theGLMatrix[ 8];myResult.m12 = (float)theGLMatrix[ 9];myResult.m22 = (float)theGLMatrix[10];myResult.m32 = (float)theGLMatrix[11];
//            myResult.m03 = (float)theGLMatrix[12];myResult.m13 = (float)theGLMatrix[13];myResult.m23 = (float)theGLMatrix[14];myResult.m33 = (float)theGLMatrix[15];
//            return myResult;
//        }
//        
//        static Matrix4<T>multiply(Matrix4<T>theM1, Matrix4<T>theM2) {
//            return new CCMatrix4f(
//                                  theM1.m00 * theM2.m00 + theM1.m01 * theM2.m10 + theM1.m02 * theM2.m20 + theM1.m03 * theM2.m30,
//                                  theM1.m00 * theM2.m01 + theM1.m01 * theM2.m11 + theM1.m02 * theM2.m21 + theM1.m03 * theM2.m31,
//                                  theM1.m00 * theM2.m02 + theM1.m01 * theM2.m12 + theM1.m02 * theM2.m22 + theM1.m03 * theM2.m32,
//                                  theM1.m00 * theM2.m03 + theM1.m01 * theM2.m13 + theM1.m02 * theM2.m23 + theM1.m03 * theM2.m33,
//                                  
//                                  theM1.m10 * theM2.m00 + theM1.m11 * theM2.m10 + theM1.m12 * theM2.m20 + theM1.m13 * theM2.m30,
//                                  theM1.m10 * theM2.m01 + theM1.m11 * theM2.m11 + theM1.m12 * theM2.m21 + theM1.m13 * theM2.m31,
//                                  theM1.m10 * theM2.m02 + theM1.m11 * theM2.m12 + theM1.m12 * theM2.m22 + theM1.m13 * theM2.m32,
//                                  theM1.m10 * theM2.m03 + theM1.m11 * theM2.m13 + theM1.m12 * theM2.m23 + theM1.m13 * theM2.m33,
//                                  
//                                  theM1.m20 * theM2.m00 + theM1.m21 * theM2.m10 + theM1.m22 * theM2.m20 + theM1.m23 * theM2.m30,
//                                  theM1.m20 * theM2.m01 + theM1.m21 * theM2.m11 + theM1.m22 * theM2.m21 + theM1.m23 * theM2.m31,
//                                  theM1.m20 * theM2.m02 + theM1.m21 * theM2.m12 + theM1.m22 * theM2.m22 + theM1.m23 * theM2.m32,
//                                  theM1.m20 * theM2.m03 + theM1.m21 * theM2.m13 + theM1.m22 * theM2.m23 + theM1.m23 * theM2.m33,
//                                  
//                                  theM1.m30 * theM2.m00 + theM1.m31 * theM2.m10 + theM1.m32 * theM2.m20 + theM1.m33 * theM2.m30,
//                                  theM1.m30 * theM2.m01 + theM1.m31 * theM2.m11 + theM1.m32 * theM2.m21 + theM1.m33 * theM2.m31,
//                                  theM1.m30 * theM2.m02 + theM1.m31 * theM2.m12 + theM1.m32 * theM2.m22 + theM1.m33 * theM2.m32,
//                                  theM1.m30 * theM2.m03 + theM1.m31 * theM2.m13 + theM1.m32 * theM2.m23 + theM1.m33 * theM2.m33
//                                  );
//        }
//        
//        static Matrix4<T>blend(float theBlend, Matrix4<T>theM1, Matrix4<T>theM2) {
//            return new CCMatrix4f(
//                                  CCMath.blend(theM1.m00, theM2.m00, theBlend),
//                                  CCMath.blend(theM1.m01, theM2.m01, theBlend),
//                                  CCMath.blend(theM1.m02, theM2.m02, theBlend),
//                                  CCMath.blend(theM1.m03, theM2.m03, theBlend),
//                                  
//                                  CCMath.blend(theM1.m10, theM2.m10, theBlend),
//                                  CCMath.blend(theM1.m11, theM2.m11, theBlend),
//                                  CCMath.blend(theM1.m12, theM2.m12, theBlend),
//                                  CCMath.blend(theM1.m13, theM2.m13, theBlend),
//                                  
//                                  CCMath.blend(theM1.m20, theM2.m20, theBlend),
//                                  CCMath.blend(theM1.m21, theM2.m21, theBlend),
//                                  CCMath.blend(theM1.m22, theM2.m22, theBlend),
//                                  CCMath.blend(theM1.m23, theM2.m23, theBlend),
//                                  
//                                  CCMath.blend(theM1.m30, theM2.m30, theBlend),
//                                  CCMath.blend(theM1.m31, theM2.m31, theBlend),
//                                  CCMath.blend(theM1.m32, theM2.m32, theBlend),
//                                  CCMath.blend(theM1.m33, theM2.m33, theBlend)
//                                  );
//        }
//        
//        /**
//         * @param theI
//         * @return
//         */
//        float[] row(int theI) {
//            float[] result = new float[] {0,0,0,0};
//            
//            switch(theI) {
//                case 0:
//                    result[0] = m00; result[1] = m10; result[2] = m20; result[3] = m30;
//                    break;
//                case 1:
//                    result[0] = m00; result[1] = m10; result[2] = m20; result[3] = m30;
//                    break;
//                case 2:
//                    result[0] = m00; result[1] = m10; result[2] = m20; result[3] = m30;
//                    break;
//                case 3:
//                    result[0] = m00; result[1] = m10; result[2] = m20; result[3] = m30;
//                    break;
//            }
//            
//            return result;
//        }
//        
//        void setPosition(CCVector3f thePosition) {
//            m03 = thePosition.x;
//            m13 = thePosition.y;
//            m23 = thePosition.z;
//        }
//        
//        CCVector3f getPosition() {
//            return new CCVector3f(m03, m13, m23);
//        }
//        
//        CCVector3f yawPitchRoll() {
//            float yaw, pitch, roll;
//            
//            pitch = CCMath.asin(-m21);
//            
//            float threshold = 0.001f; // Hardcoded constant - burn him, he's a witch
//            float test = cos(pitch);
//            
//            if (test > threshold) {
//                roll = CCMath.atan2(m01, m11);
//                yaw = CCMath.atan2(m20, m22);
//            } else {
//                roll = CCMath.atan2(-m10, m00);
//                yaw = 0.0f;
//            }
//            
//            return new CCVector3f(yaw, pitch, roll);
//        }
//        
//        void setRotationFromQuaternion(CCQuaternion q) {
//            float x = q.x;
//            float y = q.y;
//            float z = q.z;
//            float w = q.w;
//            
//            float x2 = x + x;
//            float y2 = y + y;
//            float z2 = z + z;
//            
//            float xx = x * x2; float xy = x * y2; float xz = x * z2;
//            float yy = y * y2; float yz = y * z2; float zz = z * z2;
//            float wx = w * x2; float wy = w * y2; float wz = w * z2;
//            
//            m00 = 1 - ( yy + zz );
//            m01 = xy - wz;
//            m02 = xz + wy;
//            
//            m10 = xy + wz;
//            m11 = 1 - ( xx + zz );
//            m12 = yz - wx;
//            
//            m20 = xz - wy;
//            m21 = yz + wx;
//            m22 = 1 - ( xx + yy );
//        }
//        
//        void extractRotation(Matrix4<T>m, CCVector3f s ) {
//            float invScaleX = 1 / s.x;
//            float invScaleY = 1 / s.y;
//            float invScaleZ = 1 / s.z;
//            
//            m00 = m.m00 * invScaleX;
//            m10 = m.m10 * invScaleX;
//            m20 = m.m20 * invScaleX;
//            
//            m01 = m.m01 * invScaleY;
//            m11 = m.m11 * invScaleY;
//            m21 = m.m21 * invScaleY;
//            
//            m02 = m.m02 * invScaleZ;
//            m12 = m.m12 * invScaleZ;
//            m22 = m.m22 * invScaleZ;
//            
//        }
//        
//        void multiplyScalar (float  s) {
//            m00 *= s; m01 *= s; m02 *= s; m03 *= s;
//            m10 *= s; m11 *= s; m12 *= s; m13 *= s;
//            m20 *= s; m21 *= s; m22 *= s; m23 *= s;
//            m30 *= s; m32 *= s; m32 *= s; m33 *= s;
//        }
//        
//        void multiplyScalar (CCVector3f v) {
//            m00 *= v.x; m01 *= v.y; m02 *= v.z;
//            m10 *= v.x; m11 *= v.y; m12 *= v.z;
//            m20 *= v.x; m21 *= v.y; m22 *= v.z;
//            m30 *= v.x; m31 *= v.y; m32 *= v.z;
//        }
//        
//        boolean equals(Matrix4<T>theOtherMatrix) {
//            return (
//                    m00 == theOtherMatrix.m00 && m10 == theOtherMatrix.m10 && m20 == theOtherMatrix.m20 && m30 == theOtherMatrix.m30 &&
//                    m01 == theOtherMatrix.m01 && m11 == theOtherMatrix.m11 && m21 == theOtherMatrix.m21 && m31 == theOtherMatrix.m31 &&
//                    m02 == theOtherMatrix.m02 && m12 == theOtherMatrix.m12 && m22 == theOtherMatrix.m22 && m32 == theOtherMatrix.m32 &&
//                    m03 == theOtherMatrix.m03 && m13 == theOtherMatrix.m13 && m23 == theOtherMatrix.m23 && m33 == theOtherMatrix.m33
//                    );
//        }
//        
//        static Matrix4<T>makeInvert (Matrix4<T>m1) {
//            
//            // based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
//            
//            float m00 = m1.m00; float m01 = m1.m01; float m02 = m1.m02; float m03 = m1.m03;
//            float m10 = m1.m10; float m11 = m1.m11; float m12 = m1.m12; float m13 = m1.m13;
//            float m20 = m1.m20; float m21 = m1.m21; float m22 = m1.m22; float m23 = m1.m23;
//            float m30 = m1.m30; float m31 = m1.m31; float m32 = m1.m32; float m33 = m1.m33;
//            
//            Matrix4<T>m2 = new CCMatrix4f();
//            
//            m2.m00 = m12 * m23 * m31 - m13 * m22 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 + m11 * m22 * m33;
//            m2.m01 = m03 * m22 * m31 - m02 * m23 * m31 - m03 * m21 * m32 + m01 * m23 * m32 + m02 * m21 * m33 - m01 * m22 * m33;
//            m2.m02 = m02 * m13 * m31 - m03 * m12 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 + m01 * m12 * m33;
//            m2.m03 = m03 * m12 * m21 - m02 * m13 * m21 - m03 * m11 * m22 + m01 * m13 * m22 + m02 * m11 * m23 - m01 * m12 * m23;
//            m2.m10 = m13 * m22 * m30 - m12 * m23 * m30 - m13 * m20 * m32 + m10 * m23 * m32 + m12 * m20 * m33 - m10 * m22 * m33;
//            m2.m11 = m02 * m23 * m30 - m03 * m22 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 + m00 * m22 * m33;
//            m2.m12 = m03 * m12 * m30 - m02 * m13 * m30 - m03 * m10 * m32 + m00 * m13 * m32 + m02 * m10 * m33 - m00 * m12 * m33;
//            m2.m13 = m02 * m13 * m20 - m03 * m12 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 + m00 * m12 * m23;
//            m2.m20 = m11 * m23 * m30 - m13 * m21 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 + m10 * m21 * m33;
//            m2.m21 = m03 * m21 * m30 - m01 * m23 * m30 - m03 * m20 * m31 + m00 * m23 * m31 + m01 * m20 * m33 - m00 * m21 * m33;
//            m2.m22 = m02 * m13 * m30 - m03 * m11 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 + m00 * m11 * m33;
//            m2.m23 = m03 * m11 * m20 - m01 * m13 * m20 - m03 * m10 * m21 + m00 * m13 * m21 + m01 * m10 * m23 - m00 * m11 * m23;
//            m2.m30 = m12 * m21 * m30 - m11 * m22 * m30 - m12 * m20 * m31 + m10 * m22 * m31 + m11 * m20 * m32 - m10 * m21 * m32;
//            m2.m31 = m01 * m22 * m30 - m02 * m21 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 + m00 * m21 * m32;
//            m2.m32 = m02 * m11 * m30 - m01 * m12 * m30 - m02 * m10 * m31 + m00 * m12 * m31 + m01 * m10 * m32 - m00 * m11 * m32;
//            m2.m33 = m01 * m12 * m20 - m02 * m11 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 + m00 * m11 * m22;
//            m2.multiplyScalar( 1 / m1.determinant() );
//            
//            return m2;
//            
//        };
//        
//        
//        static Matrix4<T>makeFrustum(
//                                             float theLeft, float theRight, 
//                                             float theBottom, float theTop, 
//                                             float theNear, float theFar
//                                             ) {
//            
//            Matrix4<T>m = new CCMatrix4f();
//            
//            float x = 2 * theNear / ( theRight - theLeft );
//            float y = 2 * theNear / ( theTop - theBottom );
//            
//            float a = ( theRight + theLeft ) / ( theRight - theLeft );
//            float b = ( theTop + theBottom ) / ( theTop - theBottom );
//            float c = - ( theFar + theNear ) / ( theFar - theNear );
//            float d = - 2 * theFar * theNear / ( theFar - theNear );
//            
//            m.m00 = x; m.m01 = 0; m.m02 = a;   m.m03 = 0;
//            m.m10 = 0; m.m11 = y; m.m12 = b;   m.m13 = 0;
//            m.m20 = 0; m.m21 = 0; m.m22 = c;   m.m23 = d;
//            m.m30 = 0; m.m31 = 0; m.m32 = - 1; m.m33 = 0;
//            
//            return m;
//            
//        }
//        
//        static Matrix4<T>makeYawPitchRoll( float yaw, float pitch, float roll)
//        {
//            Matrix4<T>out = new CCMatrix4f();
//            out.m00 = ( cos(roll) * cos(yaw)) + ( sin(roll) * sin(pitch) * sin(yaw) ); out.m01 = ( sin(roll) * cos(pitch) ); out.m02 = ( cos(roll) * -sin(yaw) ) + ( sin(roll) * sin(pitch) * cos(yaw) );
//            out.m10 = (-sin(roll) * cos(yaw)) + ( cos(roll) * sin(pitch) * sin(yaw) ); out.m11 = ( cos(roll) * cos(pitch) ); out.m12 = ( sin(roll) * sin(yaw) ) + ( cos(roll) * sin(pitch) * cos(yaw) );
//            out.m20 = ( cos(pitch) * sin(yaw)); out.m21 = -sin(pitch); out.m22 = ( cos(pitch) * cos(yaw) );
//            out.m33 = 1;
//            return out;
//        }
//        
//        static Matrix4<T>makePerspective(float theFov, float theAspect, float theNear, float theFar) {
//            float myTop = theNear * CCMath.tan( theFov * CCMath.PI / 360 );
//            float myBottom = - myTop;
//            float myLeft = myBottom * theAspect;
//            float myRight = myTop * theAspect;
//            
//            return makeFrustum( myLeft, myRight, myBottom, myTop, theNear, theFar );
//        }
//        
//        static Matrix4<T>makeOrtho(
//                                           float theLeft, float theRight, 
//                                           float theTop, float theBottom, 
//                                           float theNear, float theFar
//                                           ) {
//            
//            Matrix4<T>m = new CCMatrix4f();
//            
//            float w = theRight - theLeft;
//            float h = theTop - theBottom;
//            float p = theFar - theNear;
//            
//            float x = ( theRight + theLeft ) / w;
//            float y = ( theTop + theBottom ) / h;
//            float z = ( theFar + theNear ) / p;
//            
//            m.m00 = 2 / w; m.m01 = 0;     m.m02 = 0;      m.m03 = -x;
//            m.m10 = 0;     m.m11 = 2 / h; m.m12 = 0;      m.m13 = -y;
//            m.m20 = 0;     m.m21 = 0;     m.m22 = -2 / p; m.m23 = -z;
//            m.m30 = 0;     m.m31 = 0;     m.m32 = 0;      m.m33 = 1;
//            
//            return m;
//            
//        }
        
    };
    
    typedef Matrix4<float> Matrix4f;
    typedef CC_PTR< Matrix4<float> > Matrix4fPtr;
    
}; // namespace cclib

#endif /* defined(__Project__Matrix__) */
