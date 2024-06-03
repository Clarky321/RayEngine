#include <raylib.h>
#include <raygui.h>

#include "RayEngineCore\mapEditor.h"

int main()
{
	InitWindow(1200, 800, "Cube Map Editor");
	SetTargetFPS(60);

	MapEditor editor;

    while (!WindowShouldClose())
    {
        // Обновление редактора
        editor.Update();

        // Рендеринг
        BeginDrawing();
        ClearBackground(SKYBLUE);

        editor.Render();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}