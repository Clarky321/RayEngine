#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "RayEngineCore\cube.h"

class Chunk
{
public:
    Chunk() = default;
    Chunk(const Chunk& other);
    Chunk& operator=(const Chunk& other);
    void GenerateChunk(int chunkX, int chunkZ);
    void DrawChunk();
    const std::vector<Cube>& GetCubes() const;

private:
	std::vector<Cube> cubes;
	const int chunkSize = 16;
};

#endif // !CHUNK_H
