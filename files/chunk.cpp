#include "RayEngineCore\chunk.h"
#include "../../external/SimplexNoise/src/SimplexNoise.h"

Chunk::Chunk(const Chunk& other) : cubes(other.cubes) {}

Chunk& Chunk::operator=(const Chunk& other)
{
    if (this != &other)
    {
        cubes = other.cubes;
    }
    return *this;
}

void Chunk::GenerateChunk(int chunkX, int chunkZ)
{
    SimplexNoise noise;

    for (int x = 0; x < chunkSize; ++x)
    {
        for (int z = 0; z < chunkSize; ++z)
        {
            int worldX = chunkX * chunkSize + x;
            int worldZ = chunkZ * chunkSize + z;

            float heightValue = noise.fractal(4, worldX * 0.1f, worldZ * 0.1f) * 10.0f;

            for (int y = 0; y < heightValue; ++y)
            {
                cubes.emplace_back(Vector3{ (float)worldX, (float)y, (float)worldZ });
            }
        }
    }
}

void Chunk::DrawChunk()
{
    for (Cube& cube : cubes)
    {
        cube.Draw();
    }
}

const std::vector<Cube>& Chunk::GetCubes() const
{
    return cubes;
}