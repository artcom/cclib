#include "camera.h"

using namespace cclib;

CameraPtr 
Camera::create(int width, int height, float fov) {
    return CameraPtr(new Camera(width, height, fov));
}
        
Camera::Camera(int width, int height, float fov) :
    _myFov(CAMERA_DEFAULT_FOV),
    _myAspect(1.0f), _myNearClip(1.0f), _myFarClip(10000.0f), _myShotLength(0.0f),
    _myXRotation(0.0f), _myYRotation(0.0f), _myZRotation(0.0f),
    _myDelta(), _myPosition(), _myTarget(), _myUp()
{
    set(width, height, fov);
}

void 
Camera::set(int width, int height, float fov) {
    _myViewport = Viewport::create(0, 0, width, height);
    float cameraFov = fov;
    float cameraAspect = static_cast<float>(width) / static_cast<float>(height);
    float cameraX = 0;
    float cameraY = 0;
    float cameraZ = height / 2.0f / tan(cameraFov / 2.0f);

    float cameraNear = cameraZ / 10.0f;
    float cameraFar = cameraZ * 10.0f;

    set(cameraX, cameraY, cameraZ, 
            cameraX, cameraY, 0, 
            0, 1, 0,
            cameraFov, cameraAspect,
            cameraNear, cameraFar);
}

void 
Camera::set( float theCameraX, float theCameraY, float theCameraZ, 
        float theTargetX, float theTargetY, float theTargetZ,
        float theUpX, float theUpY, float theUpZ, 
        float theFov, float theAspect,
        float theNearClip, float theFarClip)
{
    _myPosition = Vector3f(theCameraX, theCameraY, theCameraZ);
    _myTarget = Vector3f(theTargetX, theTargetY, theTargetZ);
    _myUp = Vector3f(theUpX, theUpY, theUpZ);

    _myFov = theFov;
    _myAspect = theAspect;

    _myNearClip = theNearClip;
    _myFarClip = theFarClip;

    _myDelta = _myPosition - _myTarget;

    _myShotLength = _myDelta.length();

    _myYRotation = atan2(_myDelta.x(), _myDelta.z());
    _myXRotation = atan2(_myDelta.y(), sqrt(_myDelta.z() * _myDelta.z() + _myDelta.x() * _myDelta.x()));

    if (_myXRotation > M_PI_2 - CAMERA_TOL) {
        _myUp.y(0);
        _myUp.z(-1);
    }

    if (_myXRotation < CAMERA_TOL - M_PI_2) {
        _myUp.y(0);
        _myUp.z(1);
    }

    updateUp();
}

void Camera::updateUp() {
    // Describe the new vector between the camera and the target
    _myDelta = _myPosition - _myTarget;

    // Calculate the new "up" vector for the camera
    _myUp.x( -_myDelta.x() * _myDelta.y() );
    _myUp.y( _myDelta.z() * _myDelta.z() + _myDelta.x() * _myDelta.x() );
    _myUp.z( -_myDelta.z() * _myDelta.y() );

    // Normalize the "up" vector
    _myUp.normalize();

    // Calculate the roll if there is one
    if (_myZRotation != 0) {
        // Calculate the camera's X axis in world space
        Vector3f myDirection = Vector3f(
                _myDelta.y() * _myUp.z() - _myDelta.z() * _myUp.y(),
                _myDelta.x() * _myUp.z() - _myDelta.z() * _myUp.y(),
                _myDelta.x() * _myUp.y() - _myDelta.y() * _myUp.x()	
                );

        // Normalize this vector so that it can be scaled
        myDirection.normalize();

        // Perform the roll
        float myCosRoll = cos(_myZRotation);
        float mySinRoll = sin(_myZRotation);
        _myUp.x( _myUp.x() * myCosRoll + myDirection.x() * mySinRoll );
        _myUp.y( _myUp.y() * myCosRoll + myDirection.y() * mySinRoll );
        _myUp.z( _myUp.z() * myCosRoll + myDirection.z() * mySinRoll );
    }
}

void Camera::draw() {
    if (_myViewport) {
        _myViewport->draw();
    }

    // stores last matrices. not needed so far
    // updateProjectionInfos();

    glLoadIdentity();
    gluLookAt(
            _myPosition.x(), _myPosition.y(), _myPosition.z(), 
            _myTarget.x(),   _myTarget.y(),   _myTarget.z(),
            _myUp.x(),       _myUp.y(),       _myUp.z()
            );
    drawFrustum();	
}

void Camera::drawFrustum() {
    _myAspect = _myViewport->aspectRatio();

    float ymax = _myNearClip * (float) tan(_myFov / 2.0f);
    float ymin = -ymax;

    float xmin = ymin * _myAspect;
    float xmax = ymax * _myAspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( xmin, xmax, ymin, ymax, 
        // xmin + _myFrustumOffset.x(), xmax + _myFrustumOffset.x(), 
        // ymin + _myFrustumOffset.y(), ymax + _myFrustumOffset.y(), 
        _myNearClip, _myFarClip
    );
    glMatrixMode(GL_MODELVIEW);
}
