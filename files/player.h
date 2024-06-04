#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#include "RayEngineCore\world.h"

class Player
{
public:
    Player(Vector3 startPos, World& world);
    void Update();
    void Draw();
    Vector3 GetPosition() const;

private:
    Vector3 position;
    Vector3 velocity;
    BoundingBox boundingBox;
    bool isGrounded;
    World& world;

    void HandleCollision();
    void ApplyGravity();
    void Move();
};


#endif // !PLAYER_H
