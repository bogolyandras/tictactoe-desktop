

//Internal files
#include "mainwindowlogic.h"

#include "../resource/resource.h"

//Com API
#include <shobjidl.h>

//Shell API
#include <shellapi.h>

class MainWindow
{
    std::unique_ptr<MainWindowLogic> mainWindowLogic;
    bool trackingMouse = false;
    void trackMouse();

public:
    MainWindow() : m_hwnd(nullptr), m_menu(nullptr) { }

    static auto CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        MainWindow* pThis = nullptr;

        if (uMsg == WM_NCCREATE)
        {
            const auto pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            auto handle_message = pThis->HandleMessage(uMsg, wParam, lParam);
            if (handle_message.has_value())
            {
                return handle_message.value();
            }
            MessageBox(hwnd, handle_message.error().message().c_str(), L"Error", MB_ICONERROR);
            return 0;
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    static auto ClassName() { return L"TicTacToe Window Class"; }

    auto Create(const HINSTANCE hInstance) -> std::expected<void, Win32Error>
    {
        m_menu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
        const HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

        WNDCLASS wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = ClassName();
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = hIcon;

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            0, ClassName(), L"TicTacToe", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, nullptr, m_menu, GetModuleHandle(nullptr), this
        );

        if (!m_hwnd)
        {
            return Win32Error::fromLastError();
        }

        return {};
    }

    auto Window() const{ return m_hwnd; }

    auto Menu() const{ return m_menu; }

    auto HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> std::expected<LRESULT, Win32Error>;

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

auto MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> std::expected<LRESULT, Win32Error>
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            auto main_window_logic = MainWindowLogic::Create(m_hwnd);
            if (!main_window_logic.has_value())
            {
                return std::unexpected(main_window_logic.error());
            }
            mainWindowLogic = std::move(*main_window_logic);
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

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR pCmdLine,
    _In_ int nCmdShow)
{
    MainWindow mainWindow;

    if (auto mainWindowCreationResult = mainWindow.Create(hInstance); !mainWindowCreationResult.has_value())
    {
        MessageBox(nullptr,
            mainWindowCreationResult.error().message().c_str(),
            L"Window creation failed",
            MB_ICONERROR);
        return -1;
    }

    ShowWindow(mainWindow.Window(), nCmdShow);

    const HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(MENU_ACCELERATOR));

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
