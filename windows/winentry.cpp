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

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class MainWindow : public BaseWindow<MainWindow>
{

private:

    ID2D1Factory*          pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush*  pBrush;
    D2D1_ELLIPSE           ellipse;

    int32_t times = 0;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();

public:
    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), ellipse({}) { }

    PCWSTR  ClassName() const { return L"TicTacToe Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
            if (FAILED(result))
            {
                // Fail CreateWindowEx.
                return -1;
            }
        }
        return 0;
    case WM_CLOSE:
        {
            std::wstring s(L"Really quit? ");
            s += std::to_wstring(times++);
            if (MessageBox(m_hwnd, s.c_str(), L"Confirmation on exit", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(m_hwnd);
            }
            // Else: User canceled. Do nothing.
        }
        return 0;
    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_SIZE:
        Resize();
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

void MainWindow::CalculateLayout()
{
    if (pRenderTarget == NULL) {
        return;
    }
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float x = size.width / 2;
    const float y = size.height / 2;
    const float radius = min(x, y);
    ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
}

HRESULT MainWindow::CreateGraphicsResources()
{
    if (pRenderTarget != NULL)
    {
        //Don't initialize twice
        return S_OK;
    }

    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    HRESULT hr = pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &pRenderTarget);

    if (!SUCCEEDED(hr))
    {
        return hr;
    }

    const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
    hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

    if (!SUCCEEDED(hr))
    {
        return hr;
    }
    
    CalculateLayout();
    return hr;

}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (!SUCCEEDED(hr))
    {
        return;
    }

    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    pRenderTarget->BeginDraw();

    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
    pRenderTarget->FillEllipse(ellipse, pBrush);

    hr = pRenderTarget->EndDraw();
    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
        DiscardGraphicsResources();
    }
    EndPaint(m_hwnd, &ps);
    
}

void MainWindow::Resize()
{
    if (pRenderTarget == NULL)
    {
        return;
    }
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pRenderTarget->Resize(size);
    CalculateLayout();
    InvalidateRect(m_hwnd, NULL, FALSE);
}
