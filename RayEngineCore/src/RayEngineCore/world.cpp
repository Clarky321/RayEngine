#include "RayEngineCore\world.h"

#include <sstream>

void World::Update(Vector3 playerPosition)
{
    const int chunkSize = 16;

    int playerChunkX = (int)playerPosition.x / chunkSize;
    int playerChunkZ = (int)playerPosition.z / chunkSize;

    for (int x = -1; x <= 1; ++x)
    {
        for (int z = -1; z <= 1; ++z)
        {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;

            std::string chunkKey = GetChunkKey(chunkX, chunkZ);

            if (chunks.find(chunkKey) == chunks.end())
            {
                GenerateChunk(chunkX, chunkZ);
            }
        }
    }
}

void World::DrawWorld()
{
    for (auto& pair : chunks)
    {
        pair.second.DrawChunk();
    }
}

std::string World::GetChunkKey(int x, int z)
{
    std::stringstream ss;
    ss << x << "_" << z;
    return ss.str();
}

void World::GenerateChunk(int x, int z)
{
    Chunk chunk;
    chunk.GenerateChunk(x, z);
    std::string chunkKey = GetChunkKey(x, z);
    chunks[chunkKey] = chunk;
}

const std::unordered_map<std::string, Chunk>& World::GetChunks() const
{
    return chunks;
}