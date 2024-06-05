#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

#include "RayEngineCore/mapEditor.h"

//#include "RayEngineCore/mainMenu.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    // Инициализация окна
    InitWindow(screenWidth, screenHeight, "RayEngine");

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Инициализация ImGui и rlImGui
    rlImGuiSetup(true);

    //MainMenu mainMenu;
    MapEditor mapEditor;
    bool editorActive = true; // Начинаем с активного редактора карт

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Начало рендеринга кадра
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Начало нового кадра ImGui
        rlImGuiBegin();

        // Отображаем только редактор карт
        mapEditor.Update();
        mapEditor.Render();

        // Вызов функции для отображения демо-окна ImGui
        ImGui::ShowDemoWindow();

        // Завершение рендеринга кадра ImGui
        rlImGuiEnd();

        EndDrawing();
    }

    // Завершение работы с ImGui и rlImGui
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}