#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <string>
#include "RayEngineCore\chunk.h"


class World
{
public:
    void Update(Vector3 playerPosition);
    void DrawWorld();
    const std::unordered_map<std::string, Chunk>& GetChunks() const;

private:
    std::unordered_map<std::string, Chunk> chunks;
    std::string GetChunkKey(int x, int z);
    void GenerateChunk(int x, int z);
};

#endif // !WORLD_H
