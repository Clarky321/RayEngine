#ifndef CUBE_H
#define CUBE_H

#include <raylib.h>

class Cube
{
public:
	Cube(Vector3 position);

	void Draw();

	Vector3 position;

private:
	//Vector3 position;
};

#endif // !CUBE_H
