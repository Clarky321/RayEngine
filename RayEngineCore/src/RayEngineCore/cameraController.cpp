#include "RayEngineCore\cameraController.h"

CameraController::CameraController()
{
    camera.position = Vector3 { 10.0f, 10.0f, 10.0f };
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::Update()
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3CrossProduct(forward, camera.up);

    if (IsKeyDown(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, 0.2f));
    if (IsKeyDown(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, 0.2f));
    if (IsKeyDown(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(right, 0.2f));
    if (IsKeyDown(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, 0.2f));
    if (IsKeyDown(KEY_Q)) camera.position.y -= 0.2f;
    if (IsKeyDown(KEY_E)) camera.position.y += 0.2f;
}