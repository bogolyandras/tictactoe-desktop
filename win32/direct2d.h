#pragma once


#include <d2d1.h>
#include <iostream>
#pragma comment(lib, "d2d1")

namespace direct2d {

	std::unique_ptr<ID2D1Factory> create_factory();

	D2D1_SIZE_U get_size_of_window(HWND m_hwnd);

	std::unique_ptr<ID2D1HwndRenderTarget> create_render_target(ID2D1Factory* factory, HWND m_hwnd, D2D1_SIZE_U size);

	std::unique_ptr<ID2D1SolidColorBrush> create_brush(ID2D1HwndRenderTarget* renderTarget, D2D1::ColorF color);

};