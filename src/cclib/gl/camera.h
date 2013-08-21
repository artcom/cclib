#ifndef __CCLIB_CAMERA_INCLUDED__
#define __CCLIB_CAMERA_INCLUDED__

#define CAMERA_DEFAULT_FOV 1.0471975512 // 60degrees 
#define CAMERA_TOL 0.00001f

#include <cclib.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math.h>

namespace cclib {

class Viewport {

    private:
        int _myX;
        int _myY;
        int _myWidth;
        int _myHeight;

        Viewport() : _myX(0), _myY(0), _myWidth(640), _myHeight(480) 
        {};
        
    public:
        virtual ~Viewport() {};
        static ViewportPtr create(int x, int y, int width, int height) {
            ViewportPtr vp = ViewportPtr(new Viewport());
            vp->set(x, y, width, height);
            return vp;
        };
	
        void set(ViewportPtr theViewport) {
            _myX = theViewport->x();
            _myY = theViewport->y();
            _myWidth = theViewport->width();
            _myHeight = theViewport->height();
        };

        void set(int theX, int theY, int theWidth, int theHeight) {
            _myX = theX; _myY = theY;
            _myWidth = theWidth; _myHeight = theHeight;
        };

        void position(int theX, int theY){
            _myX = theX; _myY = theY;
        };
	
        Vector2i position(){
            Vector2i v(_myX, _myY);
            return v; 
        };	
	
        void dimension(int theWidth, int theHeight){
            _myWidth = theWidth;
            _myHeight = theHeight;
        };
		
        int x() { return _myX; };
        int y() { return _myY; };
        int width() { return _myWidth; };
        int height() { return _myHeight; };
        float aspectRatio() { return (float)_myWidth/_myHeight; };
        void draw() { glViewport(_myX, _myY, _myWidth, _myHeight); };
};
    
class Camera {

    public:
        static CameraPtr create(int width, int height, float fov=CAMERA_DEFAULT_FOV);
        void set(int width, int height, float fov); 
        void set( float theCameraX, float theCameraY, float theCameraZ, float theTargetX, float theTargetY, float theTargetZ,
                  float theUpX, float theUpY, float theUpZ, float theFov, float theAspect, float theNearClip, float theFarClip);

        void draw();
        void updateUp();
        void drawFrustum();

    private: 
        Camera(int width, int height, float fov);

        float _myFov;
        float _myAspect;
        float _myNearClip;
        float _myFarClip;
        float _myShotLength;
        float _myXRotation;
        float _myYRotation;
        float _myZRotation;

        Vector3f _myDelta;
        Vector3f _myPosition;
        Vector3f _myTarget;
        Vector3f _myUp;
    
        ViewportPtr _myViewport;
};
    
};

#endif

