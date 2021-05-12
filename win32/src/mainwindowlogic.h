#pragma once

#include "win32util.h"

// C++ Header files
#include <string>

//TicTacToe core library
#include <layout.h>
#include <table.h>
#include <ai/randomai.h>

class MainWindowLogic
{
private:

    HWND m_hwnd;

    std::unique_ptr<ID2D1Factory> pFactory;
    std::unique_ptr<ID2D1HwndRenderTarget> pRenderTarget;
    std::unique_ptr<ID2D1SolidColorBrush> backgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> cursorSelectedBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> mouseOverBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> lastMoveBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> highlighBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> borderBrush;
    std::unique_ptr<ID2D1SolidColorBrush> player1Brush;
    std::unique_ptr<ID2D1SolidColorBrush> player2Brush;

    int32_t times = 0;

    Layout layout;
    Table table;
    RandomAi ai;

    void handlePosition(Position p);

public:
    MainWindowLogic(HWND m_hwnd);

    void CalculateLayout();
    void OnPaint();
    void OnResize();
    void OnClose();

    void OnKeyPressUp();
    void OnKeyPressDown();
    void OnKeyPressLeft();
    void OnKeyPressRight();
    void OnKeyPressPlace();

    void OnMouseMove(int positionX, int positionY);
    void OnMouseClick(int positionX, int positionY);

};