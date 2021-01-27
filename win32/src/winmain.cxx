

//Internal files
#include "mainwindowlogic.h"
#include "basewin.h"

//Com API
#include <shobjidl.h>

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
                UNREFERENCED_PARAMETER(e);
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
        case VK_RETURN:
        case VK_SPACE:
            mainWindowLogic->OnKeyPressPlace();
            break;
        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }

        InvalidateRect(m_hwnd, NULL, TRUE);
        return 0;

    case WM_LBUTTONUP:
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
