#ifndef CUBE_H
#define CUBE_H

#include <raylib.h>

class Cube
{
public:
    Vector3 position;
    Color color;

    Cube(Vector3 pos, Color col);
    void Draw() const;
};

#endif // !CUBE_H
