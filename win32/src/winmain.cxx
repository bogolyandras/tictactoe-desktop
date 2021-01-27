#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// Windows Header Files
#include <windows.h>
#include <windowsx.h>

// C++ Header files
#include <iostream>
#include <string>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//Com API
#include <shobjidl.h>

#include "basewin.h"
#include "direct2d.h"
#include <layout.h>
#include <table.h>

class MainWindowLogic
{
private:

    HWND m_hwnd;

    std::unique_ptr<ID2D1Factory> pFactory;
    std::unique_ptr<ID2D1HwndRenderTarget> pRenderTarget;
    std::unique_ptr<ID2D1SolidColorBrush> backgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> cursorSelectedBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> mouseOverBackgroundBrush;
    std::unique_ptr<ID2D1SolidColorBrush> borderBrush;
    std::unique_ptr<ID2D1SolidColorBrush> player1Brush;
    std::unique_ptr<ID2D1SolidColorBrush> player2Brush;

    int32_t times = 0;

    Layout layout;
    Table table;

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

    void OnMouseMove(int positionX, int positionY);
    void OnMouseClick(int positionX, int positionY);

};

class MainWindow : public BaseWindow<MainWindow>
{
private:
    std::unique_ptr<MainWindowLogic> mainWindowLogic;
    bool trackingMouse = false;
    void trackMouse();

public:
    MainWindow() {}

    PCWSTR  ClassName() const { return L"TicTacToe Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

MainWindowLogic::MainWindowLogic(HWND m_hwnd) :
    m_hwnd{ m_hwnd },
    layout(35, 25),
    table(35, 25)
{
    pFactory = direct2d::create_factory();
    D2D1_SIZE_U size = direct2d::get_size_of_window(m_hwnd);
    pRenderTarget = direct2d::create_render_target(pFactory.get(), m_hwnd, size);
    backgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::White);
    cursorSelectedBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::LightBlue);
    mouseOverBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::LightGreen);
    borderBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::DarkOrange);
    player1Brush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::PaleVioletRed);
    player2Brush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::DarkGreen);
    CalculateLayout();
}

void MainWindow::trackMouse()
{
    TRACKMOUSEEVENT t = {};
    t.cbSize = sizeof(TRACKMOUSEEVENT);
    t.dwFlags = TME_LEAVE;
    t.hwndTrack = m_hwnd;
    t.dwHoverTime = HOVER_DEFAULT;
    const bool mouseTrackRegistration = TrackMouseEvent(&t);
    if (!mouseTrackRegistration) {
        throw GetLastError();
    }
    trackingMouse = true;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            try {
                mainWindowLogic.reset(new MainWindowLogic(m_hwnd));
            }
            catch (const std::exception& e) {
                return -1;
            }
        }
        return 0;
    case WM_CLOSE:
        mainWindowLogic->OnClose();
        return 0;
    case WM_DESTROY:
        mainWindowLogic.release();
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        mainWindowLogic->OnPaint();
        return 0;
    case WM_SIZE:
        mainWindowLogic->OnResize();
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            mainWindowLogic->OnKeyPressLeft();
            break;
        case VK_RIGHT:
            mainWindowLogic->OnKeyPressRight();
            break;
        case VK_DOWN:
            mainWindowLogic->OnKeyPressDown();
            break;
        case VK_UP:
            mainWindowLogic->OnKeyPressUp();
            break;
        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }

        InvalidateRect(m_hwnd, NULL, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        {
            const int xPos = GET_X_LPARAM(lParam);
            const int yPos = GET_Y_LPARAM(lParam);
            mainWindowLogic->OnMouseClick(xPos, yPos);
        }
        InvalidateRect(m_hwnd, NULL, TRUE);
        return 0;
    case WM_MOUSEMOVE:
        if (!trackingMouse) {
            trackMouse();
        }
        {
            const int xPos = GET_X_LPARAM(lParam);
            const int yPos = GET_Y_LPARAM(lParam);
            mainWindowLogic->OnMouseMove(xPos, yPos);
        }
        InvalidateRect(m_hwnd, NULL, TRUE);
        return 0;
    case WM_MOUSELEAVE:
        trackingMouse = false;
        mainWindowLogic->OnMouseMove(-1, -1);
        InvalidateRect(m_hwnd, NULL, TRUE);
        return 0;
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  PWSTR pCmdLine, _In_  int nCmdShow)
{
    MainWindow mainWindow;

    if (!mainWindow.Create(L"TicTacToe", WS_OVERLAPPEDWINDOW))
    {
        return -1;
    }

    ShowWindow(mainWindow.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


void MainWindowLogic::CalculateLayout()
{
    
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    layout.ChangeSize(size.width, size.height);

}

void MainWindowLogic::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    pRenderTarget->BeginDraw();

    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (size_t i = 0; i < layout.numberOfFields; i++) {

        D2D1_RECT_F rect = D2D1::RectF(layout.fields[i].positionX,
                                       layout.fields[i].positionY,
                                       layout.fields[i].positionX + layout.commonFieldStructure.size,
                                       layout.fields[i].positionY + layout.commonFieldStructure.size
        );
        pRenderTarget->FillRectangle(rect, borderBrush.get());

        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float minSize = std::min(size.width, size.height);

        const float border = minSize / 400;
        const float halfBorder = minSize / 800;
        rect = D2D1::RectF(layout.fields[i].positionX + halfBorder,
            layout.fields[i].positionY + halfBorder,
            layout.fields[i].positionX + layout.commonFieldStructure.size - halfBorder,
            layout.fields[i].positionY + layout.commonFieldStructure.size - halfBorder
        );

        ID2D1SolidColorBrush* selectedBackgroundBrushP;
        if (layout.fields[i].cursorOver) {
            selectedBackgroundBrushP = cursorSelectedBackgroundBrush.get();
        }
        else if (layout.fields[i].mouseOver) {
            selectedBackgroundBrushP = mouseOverBackgroundBrush.get();
        }
        else {
            selectedBackgroundBrushP = backgroundBrush.get();
        }
        
        pRenderTarget->FillRectangle(rect, selectedBackgroundBrushP);
        
        if (table.fields[i].fieldState == FieldState::Naught) {
            const float x = layout.fields[i].positionX + (layout.commonFieldStructure.size / 2.0);
            const float y = layout.fields[i].positionY + (layout.commonFieldStructure.size / 2.0);
            const float radius = std::min(layout.commonFieldStructure.size / 2.0f, layout.commonFieldStructure.size / 2.0f);

            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
            pRenderTarget->FillEllipse(ellipse, player1Brush.get());
            ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius * 0.8, radius * 0.8);
            pRenderTarget->FillEllipse(ellipse, backgroundBrush.get());
        } else if (table.fields[i].fieldState == FieldState::Cross) {
            pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY),
                D2D1::Point2F(layout.fields[i].positionX +layout.commonFieldStructure.size, layout.fields[i].positionY + layout.commonFieldStructure.size),
                    player2Brush.get(),
                1.5f
            );

            pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY + layout.commonFieldStructure.size),
                D2D1::Point2F(layout.fields[i].positionX + layout.commonFieldStructure.size, layout.fields[i].positionY),
                player2Brush.get(),
                border
            );
        }
    }
    

    HRESULT hr = pRenderTarget->EndDraw();
    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
        throw - 1;
    }
    EndPaint(m_hwnd, &ps);
    
}

void MainWindowLogic::OnResize()
{
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pRenderTarget->Resize(size);
    CalculateLayout();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindowLogic::OnClose()
{
    std::wstring s(L"Really quit? ");
    s += std::to_wstring(times++);
    if (MessageBox(m_hwnd, s.c_str(), L"Confirmation on exit", MB_OKCANCEL) == IDOK)
    {
        DestroyWindow(m_hwnd);
    }
    // Else: User canceled. Do nothing.
}

void MainWindowLogic::OnKeyPressUp()
{
    layout.OnKeyPressUp();
}

void MainWindowLogic::OnKeyPressDown()
{
    layout.OnKeyPressDown();
}

void MainWindowLogic::OnKeyPressLeft()
{
    layout.OnKeyPressLeft();
}

void MainWindowLogic::OnKeyPressRight()
{
    layout.OnKeyPressRight();
}

void MainWindowLogic::OnMouseMove(int positionX, int positionY)
{
    layout.OnMouseMove(positionX, positionY);
}

void MainWindowLogic::OnMouseClick(int positionX, int positionY)
{
    layout.OnMouseClick(positionX, positionY);
}
