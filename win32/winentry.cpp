#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Windows Header Files
#include <windows.h>

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

//Direct2D
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"
#include <layout.h>

class MainWindowLogic
{
private:

    HWND m_hwnd;

    std::unique_ptr<ID2D1Factory> pFactory;
    std::unique_ptr<ID2D1HwndRenderTarget> pRenderTarget;
    std::unique_ptr<ID2D1SolidColorBrush> pBrush;

    int32_t times = 0;

    Layout layout;

public:
    MainWindowLogic(HWND m_hwnd);

    void    CalculateLayout();
    void    OnPaint();
    void    OnResize();
    void    OnClose();

};

class MainWindow : public BaseWindow<MainWindow>
{
private:
    std::unique_ptr<MainWindowLogic> mainWindowSub;
public:
    MainWindow() { }

    PCWSTR  ClassName() const { return L"TicTacToe Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

MainWindowLogic::MainWindowLogic(HWND m_hwnd) :
    m_hwnd{ m_hwnd },
    layout(35, 25)
{
    //Initialize Direct2D factory
    ID2D1Factory* pFactoryResult = NULL;
    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactoryResult);
    if (FAILED(result))
    {
        throw - 1;
    }
    pFactory.reset(pFactoryResult);

    //Get size of window
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    //Create stuff
    ID2D1HwndRenderTarget* pRenderTargetResult = NULL;
    result = pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &pRenderTargetResult);

    if (FAILED(result))
    {
        throw - 1;
    }
    pRenderTarget.reset(pRenderTargetResult);

    ID2D1SolidColorBrush* pBrushResult = NULL;
    const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
    result = pRenderTarget->CreateSolidColorBrush(color, &pBrushResult);
    if (FAILED(result))
    {
        throw - 1;
    }
    pBrush.reset(pBrushResult);

    CalculateLayout();
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            try {
                mainWindowSub.reset(new MainWindowLogic(m_hwnd));
            }
            catch (const std::exception& e) {
                return -1;
            }
        }
        return 0;
    case WM_CLOSE:
        mainWindowSub->OnClose();
        return 0;
    case WM_DESTROY:
        mainWindowSub.release();
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        mainWindowSub->OnPaint();
        return 0;
    case WM_SIZE:
        mainWindowSub->OnResize();
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
        return 0;
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

    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

    for (size_t i = 0; i < layout.numberOfFields; i++) {
        const float x = layout.fields[i].positionX + (layout.fields[i].sizeX / 2.0);
        const float y = layout.fields[i].positionY + (layout.fields[i].sizeY / 2.0);
        const float radius = min(layout.fields[i].sizeX / 2, layout.fields[i].sizeY / 2);

        D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
        pRenderTarget->FillEllipse(ellipse, pBrush.get());
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
