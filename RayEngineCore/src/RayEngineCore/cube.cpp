#include "RayEngineCore\cube.h"

Cube::Cube(Vector3 pos, Color col) : position(pos), color(col) {}

void Cube::Draw() const
{
    DrawCube(position, 1.0f, 1.0f, 1.0f, color);
    DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
}