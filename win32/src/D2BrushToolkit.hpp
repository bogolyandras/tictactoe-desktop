#ifndef DIRECT2D_HPP
#define DIRECT2D_HPP

#include <memory>

#include <d2d1.h>
#include <expected>

#include "win32util.h"
#pragma comment(lib, "d2d1")

namespace direct2d
{
    struct  D2BrushToolkit {
        std::unique_ptr<ID2D1Factory> pFactory;
        std::unique_ptr<ID2D1HwndRenderTarget> pRenderTarget;
        std::unique_ptr<ID2D1SolidColorBrush> backgroundBrush;
        std::unique_ptr<ID2D1SolidColorBrush> cursorSelectedBackgroundBrush;
        std::unique_ptr<ID2D1SolidColorBrush> mouseOverBackgroundBrush;
        std::unique_ptr<ID2D1SolidColorBrush> lastMoveBackgroundBrush;
        std::unique_ptr<ID2D1SolidColorBrush> highLightBackgroundBrush;
        std::unique_ptr<ID2D1SolidColorBrush> borderBrush;
        std::unique_ptr<ID2D1SolidColorBrush> player1Brush;
        std::unique_ptr<ID2D1SolidColorBrush> player2Brush;
    };

    auto initialize_2d_brush_toolkit(D2BrushToolkit* brush_toolkit, HWND m_hwnd) -> std::expected<void, Win32Error>;
}

#endif //DIRECT2D_HPP
