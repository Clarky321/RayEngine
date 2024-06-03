#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <raylib.h>
#include <raymath.h>

class CameraController
{
public:
    Camera camera;

    CameraController();
    void Update();
};

#endif // !CAMERACONTROLLER_H
