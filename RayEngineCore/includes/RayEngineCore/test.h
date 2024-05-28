#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <memory>

// ������� ��� ���������� ������ � ������ ������� � ��������
void RenderModel(Model model, Vector3 position, float scale, Vector3 rotation)
{
    DrawModelEx(model, position, Vector3{ 1.0f, 0.0f, 0.0f }, rotation.x, Vector3{ scale, scale, scale }, WHITE);
}

// ������� ��� ��������� ��������������� �������������� ������
BoundingBox GetModelBoundingBox(Model model, Vector3 position, float scale, Vector3 rotation)
{
    BoundingBox bbox = GetModelBoundingBox(model);
    bbox.min = Vector3Add(Vector3Scale(bbox.min, scale), position);
    bbox.max = Vector3Add(Vector3Scale(bbox.max, scale), position);
    return bbox;
}

int main()
{
    const int screenWidth = 1024;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);

    // �������� .vox ������ � ������� raylib
    Model model = LoadModel("../../vox/T-Rex/deer.vox");
    Vector3 modelPosition = { 0.0f, 0.0f, 0.0f };
    Vector3 modelVelocity = { 0.0f, 0.0f, 0.0f };
    Vector3 modelRotation = { 0.0f, 0.0f, 0.0f };

    float modelScale = 0.1f;
    float gridSize = 50.0f;
    float moveSpeed = 8.0f;
    float gravity = 1.0f;
    float jumpSpeed = 0.5f;
    bool isGrounded = true;

    Camera camera = { 0 };
    camera.position = { 0.0f, 5.0f, 10.0f };
    camera.target = modelPosition;
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float cameraDistance = 10.0f;
    float cameraAngleY = 0.0f;
    float cameraAngleX = -PI / 4.0f;

    DisableCursor();
    SetMousePosition(screenWidth / 2, screenHeight / 2);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // ���������� ����� ������
        Vector2 mouseDelta = GetMouseDelta();
        cameraAngleY += mouseDelta.x * 0.003f;
        cameraAngleX += mouseDelta.y * 0.003f;
        cameraAngleX = Clamp(cameraAngleX, -PI / 2.0f + 0.1f, PI / 2.0f - 0.1f);

        cameraDistance -= GetMouseWheelMove() * 0.5f;
        cameraDistance = Clamp(cameraDistance, 1.0f, 50.0f);

        // ���������� ������� ������
        camera.position.x = modelPosition.x + cos(cameraAngleY) * cos(cameraAngleX) * cameraDistance;
        camera.position.z = modelPosition.z + sin(cameraAngleY) * cos(cameraAngleX) * cameraDistance;
        camera.position.y = modelPosition.y + sin(cameraAngleX) * cameraDistance;
        camera.target = modelPosition;

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        forward.y = 0.0f;
        forward = Vector3Normalize(forward);

        Vector3 right = Vector3CrossProduct(forward, camera.up);

        Vector3 movement = { 0.0f, 0.0f, 0.0f };

        if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);
        if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward);
        if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);
        if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);

        if (Vector3Length(movement) > 0.0f)
        {
            modelRotation.y = atan2(movement.x, movement.z) * RAD2DEG;
        }

        movement = Vector3Normalize(movement);
        movement = Vector3Scale(movement, moveSpeed * deltaTime);

        modelPosition.x += movement.x;
        modelPosition.z += movement.z;

        // ���������� ������
        if (isGrounded && IsKeyPressed(KEY_SPACE))
        {
            modelVelocity.y = jumpSpeed;
            isGrounded = false;
        }

        // ���������� ����������
        modelVelocity.y -= gravity * deltaTime;

        // ���������� ������� ������ � ������ ��������
        modelPosition = Vector3Add(modelPosition, modelVelocity);

        // �������� �������� � ������
        if (modelPosition.y <= 0.0f)
        {
            modelPosition.y = 0.0f;
            modelVelocity.y = 0;
            isGrounded = true;
        }
        else
        {
            isGrounded = false;
        }

        // ����������� ������� ������ � �������� �����
        modelPosition.x = Clamp(modelPosition.x, -gridSize / 2, gridSize / 2);
        modelPosition.z = Clamp(modelPosition.z, -gridSize / 2, gridSize / 2);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        RenderModel(model, modelPosition, modelScale, modelRotation);

        BoundingBox bbox = GetModelBoundingBox(model, modelPosition, modelScale, modelRotation);
        DrawBoundingBox(bbox, GREEN);

        // ��������� �����
        for (float i = -gridSize / 2; i <= gridSize / 2; i++)
        {
            DrawLine3D(Vector3{ i, 0.0f, -gridSize / 2 }, Vector3{ i, 0.0f, gridSize / 2 }, LIGHTGRAY);
            DrawLine3D(Vector3{ -gridSize / 2, 0.0f, i }, Vector3{ gridSize / 2, 0.0f, i }, LIGHTGRAY);
        }

        EndMode3D();

        // ��������� ������� �� ������ ������
        DrawLine(screenWidth / 2 - 10, screenHeight / 2, screenWidth / 2 + 10, screenHeight / 2, DARKGRAY);
        DrawLine(screenWidth / 2, screenHeight / 2 - 10, screenWidth / 2, screenHeight / 2 + 10, DARKGRAY);

        // ��������� ������
        DrawText("Use WASD to move the model, SPACE to jump", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadModel(model); // ������������ ������
    CloseWindow();

    return 0;
}