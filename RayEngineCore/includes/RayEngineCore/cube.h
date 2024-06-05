#ifndef CUBE_H
#define CUBE_H

#include <raylib.h>

class Cube
{
public:
    Vector3 position;
    //Color color;
    Texture2D texture;

    //Cube(Vector3 pos, Color col);
    Cube(Vector3 pos, Texture2D tex);
    void Draw() const;
};

#endif // !CUBE_H
