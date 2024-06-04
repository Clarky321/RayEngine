#include <raylib.h>
#include <raygui.h>

#include "RayEngineCore\mapEditor.h"
#include "RayEngineCore\mainMenu.h"

int main()
{
	InitWindow(1200, 800, "Cube Map Editor");
	
    MainMenu mainMenu;
    MapEditor editor;
    
    bool editorActive = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (!editorActive)
        {
            mainMenu.Update();
            if (mainMenu.IsRayEditorButtonPressed())
            {
                editorActive = true;
            }
            if (mainMenu.IsExitButtonPressed())
            {
                break;
            }

            mainMenu.Render();
        }
        else
        {
            editor.Update();
            BeginDrawing();
            ClearBackground(SKYBLUE);
            editor.Render();
            EndDrawing();
        }
    }

    CloseWindow();

    return 0;
}