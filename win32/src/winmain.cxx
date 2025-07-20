

//Internal files
#include "mainwindowlogic.h"

#include "../resource/resource.h"

//Com API
#include <shobjidl.h>

//Shell API
#include <shellapi.h>

class MainWindow
{
private:
    std::unique_ptr<MainWindowLogic> mainWindowLogic;
    bool trackingMouse = false;
    void trackMouse();

public:
    MainWindow() : m_hwnd(nullptr), m_menu(nullptr) { }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        MainWindow* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BOOL Create(HINSTANCE hInstance)
    {
        HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
        HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

        WNDCLASS wc = {};

        wc.lpfnWndProc = MainWindow::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = hIcon;

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            0, ClassName(), L"TicTacToe", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, nullptr, hMenu, GetModuleHandle(nullptr), this
        );

        m_menu = hMenu;

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Window() const { return m_hwnd; }
    HMENU Menu() const { return m_menu; }
    static PCTSTR ClassName() { return L"TicTacToe Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    HMENU m_menu;
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

BOOL CALLBACK AboutProcHandler(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            {
                EndDialog(hwndDlg, wParam);
                return TRUE;
            }
        case ID_VISITWEBSITE:
            {
                ShellExecute(NULL, L"open", L"https://www.bogolyandras.com", NULL, NULL, SW_SHOWNORMAL);
                return TRUE;
            }
        }
        return TRUE;
    case WM_CLOSE:
        EndDialog(hwndDlg, wParam);
        return TRUE;
    default:
        return FALSE;
    }
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
                // UNREFERENCED_PARAMETER(e);
                return -1;
            }
        }
        return 0;
    case WM_CLOSE:
        mainWindowLogic->OnClose();
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_FILE_EXIT:
                mainWindowLogic->OnClose();
                return 0;
            case ID_FILE_NEW:
                mainWindowLogic->OnNew();
                return 0;
            case ID_HELP_ABOUT:
                DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUT), m_hwnd, (DLGPROC)AboutProcHandler);
                return 0;
            case ID_OPTIONS_IAMSTARTING:
            {
                MENUITEMINFO minfo = {};
                minfo.cbSize = sizeof(MENUITEMINFO);
                minfo.fMask = MIIM_STATE;
                const bool success = GetMenuItemInfo(m_menu, ID_OPTIONS_IAMSTARTING, false, &minfo);
                if (!success) {
                    throw L"Error happened!";
                }

                const bool previousCheckedState = minfo.fState & MFS_CHECKED;

                if (previousCheckedState) {
                    minfo.fState = MFS_UNCHECKED;
                }
                else {
                    minfo.fState = MFS_CHECKED;
                }
                SetMenuItemInfo(m_menu, ID_OPTIONS_IAMSTARTING, false, &minfo);
                mainWindowLogic->OnIAmStartingTheGameChange(!previousCheckedState);

                return 0;
            }
            default:
                return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }
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

        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;

    case WM_LBUTTONUP:
        {
            const int xPos = GET_X_LPARAM(lParam);
            const int yPos = GET_Y_LPARAM(lParam);
            mainWindowLogic->OnMouseClick(xPos, yPos);
        }
        InvalidateRect(m_hwnd, nullptr, TRUE);
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
        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;
    case WM_MOUSELEAVE:
        trackingMouse = false;
        mainWindowLogic->OnMouseMove(-1, -1);
        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR pCmdLine, _In_  int nCmdShow)
{
    MainWindow mainWindow;

    if (!mainWindow.Create(hInstance))
    {
        return -1;
    }

    ShowWindow(mainWindow.Window(), nCmdShow);

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(MENU_ACCELERATOR));

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(
            mainWindow.Window(),
            hAccel,
            &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
