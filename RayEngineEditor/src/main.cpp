#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

#include "RayEngineCore/mapEditor.h"

//#include "RayEngineCore/mainMenu.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    // ������������� ����
    InitWindow(screenWidth, screenHeight, "RayEngine");

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // ������������� ImGui � rlImGui
    rlImGuiSetup(true);

    //MainMenu mainMenu;
    MapEditor mapEditor;
    bool editorActive = true; // �������� � ��������� ��������� ����

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // ������ ���������� �����
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // ������ ������ ����� ImGui
        rlImGuiBegin();

        // ���������� ������ �������� ����
        mapEditor.Update();
        mapEditor.Render();

        // ����� ������� ��� ����������� ����-���� ImGui
        ImGui::ShowDemoWindow();

        // ���������� ���������� ����� ImGui
        rlImGuiEnd();

        EndDrawing();
    }

    // ���������� ������ � ImGui � rlImGui
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}