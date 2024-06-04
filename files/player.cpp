#include <RayEngineCore\player.h>

Player::Player(Vector3 startPos, World& worldRef) : position(startPos), world(worldRef), isGrounded(false)
{
    boundingBox.min = (Vector3 { position.x - 0.5f, position.y - 1.0f, position.z - 0.5f });
    boundingBox.max = (Vector3 { position.x + 0.5f, position.y + 1.0f, position.z + 0.5f });
    velocity = { 0.0f, 0.0f, 0.0f };
}

void Player::Update()
{
    ApplyGravity();
    Move();
    HandleCollision();

    boundingBox.min = (Vector3 { position.x - 0.5f, position.y - 1.0f, position.z - 0.5f });
    boundingBox.max = (Vector3 { position.x + 0.5f, position.y + 1.0f, position.z + 0.5f });
}

void Player::Draw()
{
    DrawCube(position, 1.0f, 1.0f, 1.0f, BLUE);
    DrawCube(Vector3 { position.x, position.y + 1.0f, position.z }, 1.0f, 1.0f, 1.0f, BLUE);
    DrawBoundingBox(boundingBox, RED);
}

Vector3 Player::GetPosition() const
{
    return position;
}

void Player::ApplyGravity()
{
    if (!isGrounded)
    {
        velocity.y -= 9.8f * GetFrameTime(); // Гравитация
    }
    else
    {
        velocity.y = 0;
    }

    if (IsKeyPressed(KEY_SPACE) && isGrounded)
    {
        velocity.y = 5.0f; // Прыжок
        isGrounded = false;
    }
}

void Player::Move()
{
    float speed = 5.0f * GetFrameTime();
    Vector3 moveDir = { 0.0f, 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) moveDir.z -= speed;
    if (IsKeyDown(KEY_S)) moveDir.z += speed;
    if (IsKeyDown(KEY_A)) moveDir.x -= speed;
    if (IsKeyDown(KEY_D)) moveDir.x += speed;

    position.x += moveDir.x;
    position.y += velocity.y * GetFrameTime();
    position.z += moveDir.z;
}

void Player::HandleCollision()
{
    isGrounded = false;

    for (const auto& chunkPair : world.GetChunks())
    {
        for (const Cube& cube : chunkPair.second.GetCubes())
        {
            BoundingBox cubeBoundingBox = {
                {cube.position.x - 0.5f, cube.position.y - 0.5f, cube.position.z - 0.5f},
                {cube.position.x + 0.5f, cube.position.y + 0.5f, cube.position.z + 0.5f}
            };

            if (CheckCollisionBoxes(boundingBox, cubeBoundingBox)) {
                // Handle collision on Y axis (up and down)
                if (velocity.y > 0 && boundingBox.min.y < cubeBoundingBox.max.y && boundingBox.max.y > cubeBoundingBox.min.y)
                {
                    position.y = cubeBoundingBox.min.y - 1.0f;
                    velocity.y = 0;
                }
                else if (velocity.y < 0 && boundingBox.max.y > cubeBoundingBox.min.y && boundingBox.min.y < cubeBoundingBox.max.y)
                {
                    position.y = cubeBoundingBox.max.y + 1.0f;
                    isGrounded = true;
                    velocity.y = 0;
                }

                // Handle collision on X axis (left and right)
                if (velocity.x > 0 && boundingBox.min.x < cubeBoundingBox.max.x && boundingBox.max.x > cubeBoundingBox.min.x)
                {
                    position.x = cubeBoundingBox.min.x - 0.5f;
                    velocity.x = 0;
                }
                else if (velocity.x < 0 && boundingBox.max.x > cubeBoundingBox.min.x && boundingBox.min.x < cubeBoundingBox.max.x)
                {
                    position.x = cubeBoundingBox.max.x + 0.5f;
                    velocity.x = 0;
                }

                // Handle collision on Z axis (front and back)
                if (velocity.z > 0 && boundingBox.min.z < cubeBoundingBox.max.z && boundingBox.max.z > cubeBoundingBox.min.z)
                {
                    position.z = cubeBoundingBox.min.z - 0.5f;
                    velocity.z = 0;
                }
                else if (velocity.z < 0 && boundingBox.max.z > cubeBoundingBox.min.z && boundingBox.min.z < cubeBoundingBox.max.z)
                {
                    position.z = cubeBoundingBox.max.z + 0.5f;
                    velocity.z = 0;
                }
            }
        }
    }
}