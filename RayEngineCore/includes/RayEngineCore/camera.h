#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include "RayEngineCore\player.h"

class CameraController
{
public:
    CameraController(Player& player);

    void Update();

    Camera GetCamera() const;

private:
    Camera camera;

    Player& player;

    float rotationAngle;

    void UpdateCameraPosition();
};

#endif // !CAMERA_H
