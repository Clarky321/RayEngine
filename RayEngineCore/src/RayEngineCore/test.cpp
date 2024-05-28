#include "raylib.h"
#include"raymath.h"

const int gridSize = 25;
const float cubeSize = 1.0f;
const float gravity = 9.81f;
const float jumpForce = 5.0f;
const float moveSpeed = 5.0f;
const float zoomSpeed = 1.0f;
const float rotationSpeed = 45.0f;

Vector3 cubePosition = { 0.0f, cubeSize / 2.0f, 0.0f };
Vector3 cubeVelocity = { 0.0f, 0.0f, 0.0f };
bool isGrounded = true;

void DrawCustomGrid(int size, float spacing)
{
    for (int i = -size; i <= size; i++)
    {
        DrawLine3D(Vector3{ (float)i * spacing, 0.0f, (float)-size * spacing },
            Vector3{ (float)i * spacing, 0.0f, (float)size * spacing }, GRAY);
        DrawLine3D(Vector3{ (float)-size * spacing, 0.0f, (float)i * spacing },
            Vector3{ (float)size * spacing, 0.0f, (float)i * spacing }, GRAY);
    }
}

void UpdateCubePosition(float deltaTime)
{
    if (IsKeyDown(KEY_S)) cubePosition.z += moveSpeed * deltaTime;
    if (IsKeyDown(KEY_W)) cubePosition.z -= moveSpeed * deltaTime;
    if (IsKeyDown(KEY_A)) cubePosition.x -= moveSpeed * deltaTime;
    if (IsKeyDown(KEY_D)) cubePosition.x += moveSpeed * deltaTime;

    if (isGrounded && IsKeyPressed(KEY_SPACE))
    {
        cubeVelocity.y = jumpForce;
        isGrounded = false;
    }

    // Apply gravity
    cubeVelocity.y -= gravity * deltaTime;
    cubePosition.y += cubeVelocity.y * deltaTime;

    // Check for ground collision
    if (cubePosition.y <= cubeSize / 2.0f)
    {
        cubePosition.y = cubeSize / 2.0f;
        isGrounded = true;
        cubeVelocity.y = 0.0f;
    }

    // Boundary collision detection
    if (cubePosition.x < -gridSize * cubeSize / 2.0f + cubeSize / 2.0f) cubePosition.x = -gridSize * cubeSize / 2.0f + cubeSize / 2.0f;
    if (cubePosition.x > gridSize * cubeSize / 2.0f - cubeSize / 2.0f) cubePosition.x = gridSize * cubeSize / 2.0f - cubeSize / 2.0f;
    if (cubePosition.z < -gridSize * cubeSize / 2.0f + cubeSize / 2.0f) cubePosition.z = -gridSize * cubeSize / 2.0f + cubeSize / 2.0f;
    if (cubePosition.z > gridSize * cubeSize / 2.0f - cubeSize / 2.0f) cubePosition.z = gridSize * cubeSize / 2.0f - cubeSize / 2.0f;
}

int main(void) {
    InitWindow(800, 600, "Voxel Game Test");

    SetTargetFPS(60);

    Camera camera = { 0 };
    camera.position = Vector3{ 0.0f, 20.0f, 20.0f };
    camera.target = cubePosition;
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 35.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float cameraDistance = 20.0f;
    float cameraRotation = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update cube position
        UpdateCubePosition(deltaTime);

        // Update camera position to follow the cube
        camera.position.x = cubePosition.x + cameraDistance * cos(DEG2RAD * cameraRotation);
        camera.position.z = cubePosition.z + cameraDistance * sin(DEG2RAD * cameraRotation);
        camera.position.y = cubePosition.y + cameraDistance / 1.0f;  // Adjust camera height
        camera.target = cubePosition;

        // Camera zoom
        cameraDistance -= GetMouseWheelMove() * zoomSpeed;
        if (cameraDistance < 5.0f) cameraDistance = 5.0f;  // Minimum zoom distance

        // Camera rotation
        if (IsKeyDown(KEY_Q)) cameraRotation -= rotationSpeed * deltaTime;
        if (IsKeyDown(KEY_E)) cameraRotation += rotationSpeed * deltaTime;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawCustomGrid(gridSize, cubeSize);
        DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, RED);
        DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, MAROON);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}