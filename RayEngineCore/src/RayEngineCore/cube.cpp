#include "RayEngineCore\cube.h"

Cube::Cube(Vector3 pos) : position(pos) {}

void Cube::Draw()
{
	DrawCube(position, 1.0f, 1.0f, 1.0f, DARKGREEN);
	DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
}