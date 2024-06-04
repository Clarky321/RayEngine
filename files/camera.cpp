#include "RayEngineCore\camera.h"
#include <raymath.h>

CameraController::CameraController(Player& player) : player(player), rotationAngle(0.0f)
{
    camera.position = (Vector3 { 0.0f, 10.0f, 10.0f });
    camera.target = player.GetPosition();
    camera.up = (Vector3 { 0.0f, 1.0f, 0.0f });
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::Update()
{
    if (IsKeyDown(KEY_Q)) rotationAngle -= 90.0f * GetFrameTime();
    if (IsKeyDown(KEY_E)) rotationAngle += 90.0f * GetFrameTime();

    UpdateCameraPosition();
}

void CameraController::UpdateCameraPosition()
{
    Vector3 playerPos = player.GetPosition();

    float radius = 20.0f;

    camera.position.x = playerPos.x + radius * cos(rotationAngle * DEG2RAD);
    camera.position.z = playerPos.z + radius * sin(rotationAngle * DEG2RAD);
    camera.position.y = playerPos.y + 15.0f;
    camera.target = playerPos;
}

Camera CameraController::GetCamera() const
{
    return camera;
}