#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <raylib.h>
#include <vector>

#include "RayEngineCore\cube.h"
#include "RayEngineCore\cameraController.h"

class MapEditor
{
private:
    std::vector<Cube> cubes;
    Vector3 currentCubePosition;
    Color currentColor;
    CameraController cameraController;

    Vector3 GetMousePositionInWorld();
    Vector3 SnapToGrid(Vector3 position);
    Vector3 GetTopCubePosition(Vector3 position);

public:
    MapEditor();
    void Update();
    void Render();
};

#endif // !MAPEDITOR_H
