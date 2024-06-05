#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <imgui.h>
#include "RayEngineCore\mapEditor.h"

bool CheckCollisionPointCube(Vector3 point, Vector3 cubePosition, float cubeSize)
{
    return (point.x >= cubePosition.x - cubeSize / 2 &&
        point.x <= cubePosition.x + cubeSize / 2 &&
        point.y >= cubePosition.y - cubeSize / 2 &&
        point.y <= cubePosition.y + cubeSize / 2 &&
        point.z >= cubePosition.z - cubeSize / 2 &&
        point.z <= cubePosition.z + cubeSize / 2);
}

//MapEditor::MapEditor() : currentCubePosition({ 0, 0, 0 }), currentColor(RED) {}

MapEditor::MapEditor() : currentCubePosition({ 0, 0, 0 }), currentTexture(LoadTexture("../../../assets/log_jungle.png")) {}

MapEditor::~MapEditor()
{
    UnloadTexture(currentTexture); // Очистка текстуры
}

void MapEditor::Update()
{
    cameraController.Update();

    Vector3 mousePosition = GetMousePositionInWorld();
    currentCubePosition = SnapToGrid(mousePosition);
    Vector3 topCubePosition = GetTopCubePosition(currentCubePosition);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        bool positionOccupied = false;
        for (const auto& cube : cubes)
        {
            if (Vector3Distance(cube.position, topCubePosition) < 0.1f)
            {
                positionOccupied = true;
                break;
            }
        }

        if (!positionOccupied)
        {
            cubes.push_back(Cube(topCubePosition, currentTexture));
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        Vector3 closestCubePosition = GetClosestCubePosition(mousePosition);
        if (closestCubePosition.y >= 0)
        {
            for (auto it = cubes.begin(); it != cubes.end(); ++it)
            {
                if (Vector3Distance(it->position, closestCubePosition) < 0.1f)
                {
                    cubes.erase(it);
                    break;
                }
            }
        }
    }

    //if (IsKeyPressed(KEY_ONE)) currentColor = RED;
    //if (IsKeyPressed(KEY_TWO)) currentColor = GREEN;
    //if (IsKeyPressed(KEY_THREE)) currentColor = BLUE;
}

void MapEditor::Render()
{
    BeginMode3D(cameraController.camera);

    for (const auto& cube : cubes)
    {
        cube.Draw();
    }

    DrawCubeWires(currentCubePosition, 1.0f, 1.0f, 1.0f, BLACK);

    Vector3 closestCubePosition = GetClosestCubePosition(GetMousePositionInWorld());
    if (closestCubePosition.y >= 0)
    {
        DrawCubeWires(closestCubePosition, 1.0f, 1.0f, 1.0f, YELLOW);
    }

    EndMode3D();

    GuiLabel(Rectangle { 10, 10, 200, 20 }, "Press left mouse button to place cube");
    GuiLabel(Rectangle { 10, 40, 200, 20 }, "Press right mouse button to remove cube");
    GuiLabel(Rectangle { 10, 70, 200, 20 }, "Press 1, 2, 3 to change color");

    ImGui::Begin("Demo Window");
    ImGui::Text("Hello from the Map Editor!");
    ImGui::ShowDemoWindow();
    ImGui::End();
}

Vector3 MapEditor::GetMousePositionInWorld()
{
    Vector2 mousePosition = GetMousePosition();
    Ray ray = GetMouseRay(mousePosition, cameraController.camera);

    float distance = (0.0f - ray.position.y) / ray.direction.y;
    Vector3 position = { ray.position.x + ray.direction.x * distance,
                         0.0f,
                         ray.position.z + ray.direction.z * distance };

    for (const auto& cube : cubes)
    {
        BoundingBox box = {
            { cube.position.x - 0.5f, cube.position.y - 0.5f, cube.position.z - 0.5f },
            { cube.position.x + 0.5f, cube.position.y + 0.5f, cube.position.z + 0.5f }
        };

        RayCollision collision = GetRayCollisionBox(ray, box);
        if (collision.hit)
        {
            position = collision.point;
            break;
        }
    }

    return position;
}

Vector3 MapEditor::SnapToGrid(Vector3 position)
{
    return Vector3{ (float)round(position.x), (float)round(position.y), (float)round(position.z) };
}

Vector3 MapEditor::GetTopCubePosition(Vector3 position)
{
    Vector3 topPosition = position;
    topPosition.y = 0.0f;

    for (const auto& cube : cubes)
    {
        if (cube.position.x == position.x && cube.position.z == position.z)
        {
            if (cube.position.y >= topPosition.y)
            {
                topPosition.y = cube.position.y + 1.0f;
            }
        }
    }
    return topPosition;
}

Vector3 MapEditor::GetClosestCubePosition(Vector3 position)
{
    Vector3 closestPosition = { 0, -1, 0 }; // Default to invalid position
    float closestDistance = FLT_MAX;

    for (const auto& cube : cubes)
    {
        float distance = Vector3Distance(position, cube.position);
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPosition = cube.position;
        }
    }

    return closestPosition;
}