#include <raylib.h>
#include <raymath.h>
#include <RayEngineCore/vox_file.h>
#include <vector>
#include <iostream>
#include <memory>

// Функция для рендеринга модели с учётом позиции и вращения
void RenderModel(const magicavoxel::VoxDenseModel& model, Vector3 position, float scale, Vector3 rotation)
{
    for (uint32_t z = 0; z < model.size().z; ++z)
    {
        for (uint32_t y = 0; y < model.size().y; ++y)
        {
            for (uint32_t x = 0; x < model.size().x; ++x)
            {
                uint8_t colorIndex = model.voxel(x, y, z);
                if (colorIndex != 0)
                {
                    magicavoxel::Color color = model.palette()[colorIndex];
                    Color raylibColor = { color.r, color.g, color.b, color.a };
                    Vector3 voxelPosition = { x * scale, y * scale, z * scale };

                    // Применение вращения
                    Matrix rotationMatrix = MatrixRotateXYZ(Vector3 { DEG2RAD* rotation.x, DEG2RAD* rotation.y, DEG2RAD* rotation.z });
                    voxelPosition = Vector3Transform(voxelPosition, rotationMatrix);

                    // Смещение модели для корректного отображения
                    voxelPosition = Vector3Add(voxelPosition, position);

                    DrawCube(voxelPosition, scale, scale, scale, raylibColor);
                }
            }
        }
    }
}

// Функция для получения ограничивающего прямоугольника модели
BoundingBox GetModelBoundingBox(const magicavoxel::VoxDenseModel& model, Vector3 position, float scale, Vector3 rotation)
{
    Vector3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
    Vector3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (uint32_t z = 0; z < model.size().z; ++z)
    {
        for (uint32_t y = 0; y < model.size().y; ++y)
        {
            for (uint32_t x = 0; x < model.size().x; ++x)
            {
                uint8_t colorIndex = model.voxel(x, y, z);
                if (colorIndex != 0)
                {
                    Vector3 voxelPosition = { x * scale, y * scale, z * scale };

                    // Применение вращения
                    Matrix rotationMatrix = MatrixRotateXYZ(Vector3 { DEG2RAD* rotation.x, DEG2RAD* rotation.y, DEG2RAD* rotation.z });
                    voxelPosition = Vector3Transform(voxelPosition, rotationMatrix);

                    // Смещение модели для корректного отображения
                    voxelPosition = Vector3Add(voxelPosition, position);

                    min = Vector3Min(min, voxelPosition);
                    max = Vector3Max(max, voxelPosition);
                }
            }
        }
    }

    return (BoundingBox { min, max });
}

int main()
{
    const int screenWidth = 1024;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);

    std::vector<std::vector<std::unique_ptr<magicavoxel::VoxDenseModel>>> animationFrames;
    try
    {
        magicavoxel::VoxFile voxFile(true, true, true);
        voxFile.Load("../../vox/T-Rex/deer.vox");

        for (auto& frame : voxFile.denseModels())
        {
            std::vector<std::unique_ptr<magicavoxel::VoxDenseModel>> frameModels;
            frameModels.push_back(std::make_unique<magicavoxel::VoxDenseModel>(std::move(frame)));
            animationFrames.push_back(std::move(frameModels));
        }
    }
    catch (const magicavoxel::VoxException& e)
    {
        std::cerr << "Error loading VOX files: " << e.what() << std::endl;
        return -1;
    }

    Vector3 modelPosition = { 0.0f, 0.0f, 0.0f };
    Vector3 modelVelocity = { 0.0f, 0.0f, 0.0f };
    Vector3 modelRotation = { -90.0f, 0.0f, 0.0f };
    float modelScale = 0.1f;
    float gridSize = 50.0f;
    float moveSpeed = 8.0f;
    float gravity = 1.0f;
    float jumpSpeed = 0.5f;
    bool isGrounded = true;

    Camera camera = { 0 };
    camera.position = { 0.0f, 5.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float cameraDistance = 10.0f;
    float cameraAngleY = 0.0f;
    float cameraAngleX = -PI / 4.0f;

    DisableCursor();
    SetMousePosition(screenWidth / 2, screenHeight / 2);

    float frameTime = 0.1f; // Время показа одного кадра анимации
    float frameCounter = 0.0f;
    int currentFrame = 0;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Обновление углов камеры
        Vector2 mouseDelta = GetMouseDelta();
        cameraAngleY += mouseDelta.x * 0.003f;
        cameraAngleX += mouseDelta.y * 0.003f;
        cameraAngleX = Clamp(cameraAngleX, -PI / 2.0f + 0.1f, PI / 2.0f - 0.1f);

        cameraDistance -= GetMouseWheelMove() * 0.5f;
        cameraDistance = Clamp(cameraDistance, 1.0f, 50.0f);

        camera.position.x = modelPosition.x + cos(cameraAngleY) * cos(cameraAngleX) * cameraDistance;
        camera.position.z = modelPosition.z + sin(cameraAngleY) * cos(cameraAngleX) * cameraDistance;
        camera.position.y = modelPosition.y + sin(cameraAngleX) * cameraDistance;
        camera.target = Vector3Add(modelPosition, Vector3{ 0.0f, 0.5f * modelScale * animationFrames[currentFrame][0]->size().y, 0.0f });

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

        // Добавление прыжка
        if (isGrounded && IsKeyPressed(KEY_SPACE))
        {
            modelVelocity.y = jumpSpeed;
            isGrounded = false;
        }

        // Применение гравитации
        modelVelocity.y -= gravity * deltaTime;

        // Обновление позиции модели с учетом скорости
        modelPosition = Vector3Add(modelPosition, modelVelocity);

        // Проверка коллизии с землей
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

        // Ограничение позиции модели в пределах сетки
        modelPosition.x = Clamp(modelPosition.x, -gridSize / 2, gridSize / 2);
        modelPosition.z = Clamp(modelPosition.z, -gridSize / 2, gridSize / 2);

        // Обновление текущего кадра анимации
        frameCounter += deltaTime;
        if (frameCounter >= frameTime)
        {
            frameCounter = 0.0f;
            currentFrame = (currentFrame + 1) % animationFrames.size();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        if (!animationFrames.empty() && !animationFrames[currentFrame].empty())
        {
            RenderModel(*animationFrames[currentFrame][0], modelPosition, modelScale, modelRotation);

            BoundingBox bbox = GetModelBoundingBox(*animationFrames[currentFrame][0], modelPosition, modelScale, modelRotation);
            DrawBoundingBox(bbox, GREEN);
        }

        // Рисование сетки
        for (float i = -gridSize / 2; i <= gridSize / 2; i++)
        {
            DrawLine3D(Vector3{ i, 0.0f, -gridSize / 2 }, Vector3{ i, 0.0f, gridSize / 2 }, LIGHTGRAY);
            DrawLine3D(Vector3{ -gridSize / 2, 0.0f, i }, Vector3{ gridSize / 2, 0.0f, i }, LIGHTGRAY);
        }

        EndMode3D();

        // Рисование текста
        DrawText("Use WASD to move the model, SPACE to jump", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}