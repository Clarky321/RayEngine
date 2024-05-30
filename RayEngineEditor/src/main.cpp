#include <raylib.h>

#include "RayEngineCore\world.h"
#include "RayEngineCore\player.h"
#include "RayEngineCore\camera.h"

int main()
{
	const int screenWidth = 1200;
	const int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Isometric_Voxel");

    World world;

    Player player(Vector3 {0.0f, 10.0f, 0.0f}, world);
    CameraController camera(player);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        player.Update();
        camera.Update();
        world.Update(player.GetPosition());

        // Рисование
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        BeginMode3D(camera.GetCamera());

        world.DrawWorld();
        player.Draw();

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}