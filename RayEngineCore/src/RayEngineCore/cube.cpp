#include "RayEngineCore/cube.h"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>



Cube::Cube(Vector3 pos, Texture2D tex) : position(pos), texture(tex) {}

void Cube::Draw() const
{
    
    
    // Размер куба
    float size = 1.0f;

    // Координаты вершин куба
    Vector3 vertices[8] = {
        {position.x - size / 2, position.y - size / 2, position.z - size / 2},
        {position.x + size / 2, position.y - size / 2, position.z - size / 2},
        {position.x + size / 2, position.y + size / 2, position.z - size / 2},
        {position.x - size / 2, position.y + size / 2, position.z - size / 2},
        {position.x - size / 2, position.y - size / 2, position.z + size / 2},
        {position.x + size / 2, position.y - size / 2, position.z + size / 2},
        {position.x + size / 2, position.y + size / 2, position.z + size / 2},
        {position.x - size / 2, position.y + size / 2, position.z + size / 2},
    };

    // Индексы вершин для рисования граней куба
    int faces[6][4] = {
        {0, 1, 2, 3}, // Front face
        {4, 5, 6, 7}, // Back face
        {0, 1, 5, 4}, // Bottom face
        {2, 3, 7, 6}, // Top face
        {0, 3, 7, 4}, // Left face
        {1, 2, 6, 5}  // Right face
    };

    // Рисуем каждую грань куба
    for (int i = 0; i < 6; i++)
    {
        Vector3 v1 = vertices[faces[i][0]];
        Vector3 v2 = vertices[faces[i][1]];
        Vector3 v3 = vertices[faces[i][2]];
        Vector3 v4 = vertices[faces[i][3]];

        Vector3 normal = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(v2, v1), Vector3Subtract(v3, v1)));

        //BeginMode3D();
        rlPushMatrix();
        rlTranslatef(position.x, position.y, position.z);
        rlBegin(RL_QUADS);
        rlNormal3f(normal.x, normal.y, normal.z);
        rlTexCoord2f(0.0f, 1.0f);
        rlVertex3f(v1.x, v1.y, v1.z);
        rlTexCoord2f(1.0f, 1.0f);
        rlVertex3f(v2.x, v2.y, v2.z);
        rlTexCoord2f(1.0f, 0.0f);
        rlVertex3f(v3.x, v3.y, v3.z);
        rlTexCoord2f(0.0f, 0.0f);
        rlVertex3f(v4.x, v4.y, v4.z);
        rlEnd();
        rlPopMatrix();
        EndMode3D();
    }

    // Рисуем проволочный каркас куба
    DrawCubeWires(position, size, size, size, BLACK);
}
