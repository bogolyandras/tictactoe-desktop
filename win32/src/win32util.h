#ifndef WIN32_UTIL_H
#define WIN32_UTIL_H

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Windows Header Files
#include <windows.h>
#include <windowsx.h>

#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include <memory>

namespace direct2d {

	std::unique_ptr<ID2D1Factory> create_factory();

	D2D1_SIZE_U get_size_of_window(HWND m_hwnd);

	std::unique_ptr<ID2D1HwndRenderTarget> create_render_target(ID2D1Factory* factory, HWND m_hwnd, D2D1_SIZE_U size);

	std::unique_ptr<ID2D1SolidColorBrush> create_brush(ID2D1HwndRenderTarget* renderTarget, D2D1::ColorF color);

};

#endif //WIN32_UTIL_H