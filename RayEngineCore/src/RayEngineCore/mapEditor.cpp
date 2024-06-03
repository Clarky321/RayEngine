#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

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

MapEditor::MapEditor() : currentCubePosition({ 0, 0, 0 }), currentColor(RED) {}

void MapEditor::Update()
{
    cameraController.Update();

    Vector3 mousePosition = GetMousePositionInWorld();
    currentCubePosition = SnapToGrid(mousePosition);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector3 topCubePosition = GetTopCubePosition(currentCubePosition);
        bool positionOccupied = false;

        // Check if the top position is already occupied
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
            cubes.push_back(Cube(topCubePosition, currentColor));
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        for (auto it = cubes.begin(); it != cubes.end(); ++it)
        {
            if (CheckCollisionPointCube(mousePosition, it->position, 1.0f))
            {
                cubes.erase(it);
                break;
            }
        }
    }

    if (IsKeyPressed(KEY_ONE)) currentColor = RED;
    if (IsKeyPressed(KEY_TWO)) currentColor = GREEN;
    if (IsKeyPressed(KEY_THREE)) currentColor = BLUE;
}

void MapEditor::Render()
{
    BeginMode3D(cameraController.camera);

    for (const auto& cube : cubes)
    {
        cube.Draw();
    }

    DrawCubeWires(currentCubePosition, 1.0f, 1.0f, 1.0f, BLACK);

    EndMode3D();

    GuiLabel(Rectangle { 10, 10, 200, 20 }, "Press left mouse button to place cube");
    GuiLabel(Rectangle { 10, 40, 200, 20 }, "Press right mouse button to remove cube");
    GuiLabel(Rectangle { 10, 70, 200, 20 }, "Press 1, 2, 3 to change color");
}

Vector3 MapEditor::GetMousePositionInWorld()
{
    Vector2 mousePosition = GetMousePosition();
    Ray ray = GetMouseRay(mousePosition, cameraController.camera);

    float groundLevel = 0.0f; // Y-координата уровня земли
    float distance = (groundLevel - ray.position.y) / ray.direction.y;
    Vector3 position = { ray.position.x + ray.direction.x * distance,
                         groundLevel,
                         ray.position.z + ray.direction.z * distance };
    return position;
}

Vector3 MapEditor::SnapToGrid(Vector3 position)
{
    return Vector3{ (float)round(position.x), (float)round(position.y), (float)round(position.z) };
}

Vector3 MapEditor::GetTopCubePosition(Vector3 position)
{
    Vector3 topPosition = position;
    topPosition.y = 0.0f; // Start at ground level

    // Check if there is a cube directly below the current position
    for (const auto& cube : cubes)
    {
        if (cube.position.x == position.x && cube.position.z == position.z)
        {
            // If a cube is found directly below, place the new block on top of it
            if (cube.position.y >= topPosition.y)
            {
                topPosition.y = cube.position.y + 1.0f;
            }
        }
    }

    return topPosition;
}