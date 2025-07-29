#ifndef MAINWINDOWLOGIC_H
#define MAINWINDOWLOGIC_H

#include "win32util.h"
#include "D2BrushToolkit.hpp"

// C++ Header files
#include <string>

//TicTacToe core library
#include <layout.h>
#include <table.h>
#include <ai/heuristicai.h>

class MainWindowLogic
{
    MainWindowLogic() = delete;
    explicit MainWindowLogic(HWND m_hwnd);
    HWND m_hwnd;

    direct2d::D2BrushToolkit brush_toolkit;

    int32_t times = 0;

    Layout layout;
    Table table;
    HeuristicAi ai;
    bool IAmStartingTheGame;

    void handlePosition(Position p);
    void handleOpponent();
public:
    static auto Create(HWND m_hwnd) -> std::expected<std::unique_ptr<MainWindowLogic>, Win32Error>;

    void CalculateLayout();
    void OnPaint();
    void OnResize();
    void OnClose();

    void OnKeyPressUp();
    void OnKeyPressDown();
    void OnKeyPressLeft();
    void OnKeyPressRight();
    void OnKeyPressPlace();

    void OnNew();
    void OnIAmStartingTheGameChange(bool newValue);

    void OnMouseMove(int positionX, int positionY);
    void OnMouseClick(int positionX, int positionY);

};

#endif // MAINWINDOWLOGIC_H