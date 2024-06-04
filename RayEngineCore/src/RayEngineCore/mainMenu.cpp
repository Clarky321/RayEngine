#include "RayEngineCore\mainMenu.h"

MainMenu::MainMenu() : rayEditorButtonPressed(false), exitButtonPressed(false) {}

void MainMenu::Update()
{
    rayEditorButtonPressed = false;
    exitButtonPressed = false;

    Vector2 mousePoint = GetMousePosition();
    Rectangle rayEditorButton = { GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 2.0f - 50, 150, 50 };
    Rectangle exitButton = { GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 2.0f + 10, 150, 50 };

    if (GuiButton(rayEditorButton, "RayEditor"))
    {
        rayEditorButtonPressed = true;
    }

    if (GuiButton(exitButton, "Exit"))
    {
        exitButtonPressed = true;
    }
}

void MainMenu::Render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    const char* titleText = "RayEngine";
    const int fontSize = 40;
    Vector2 titlePosition = { (GetScreenWidth() - MeasureText(titleText, fontSize)) / 2.0f, GetScreenHeight() / 4.0f - fontSize / 2.0f };
    DrawText(titleText, titlePosition.x, titlePosition.y, fontSize, BLACK);

    Vector2 mousePoint = GetMousePosition();
    Rectangle rayEditorButton = { GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 2.0f - 50, 150, 50 };
    Rectangle exitButton = { GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 2.0f + 10, 150, 50 };

    GuiButton(rayEditorButton, "RayEditor");
    GuiButton(exitButton, "Exit");

    EndDrawing();
}

bool MainMenu::IsRayEditorButtonPressed() const
{
    return rayEditorButtonPressed;
}

bool MainMenu::IsExitButtonPressed() const
{
    return exitButtonPressed;
}