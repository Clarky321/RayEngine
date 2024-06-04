#ifndef MAINMENU_H
#define MAINMENU_H

#include <raylib.h>
#include <raygui.h>

class MainMenu
{
public:
    MainMenu();
    void Update();
    void Render();
    bool IsRayEditorButtonPressed() const;
    bool IsExitButtonPressed() const;

private:
    bool rayEditorButtonPressed;
    bool exitButtonPressed;
};

#endif // MAINMENU_H