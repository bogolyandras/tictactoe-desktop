#include "win32util.h"

std::unique_ptr<ID2D1Factory> direct2d::create_factory()
{
    ID2D1Factory* pFactoryResult = NULL;
    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactoryResult);
    if (FAILED(result))
    {
        throw - 1;
    }
	return std::unique_ptr<ID2D1Factory>(pFactoryResult);
}

D2D1_SIZE_U direct2d::get_size_of_window(HWND m_hwnd)
{
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    return D2D1::SizeU(rc.right, rc.bottom);
}

std::unique_ptr<ID2D1HwndRenderTarget> direct2d::create_render_target(ID2D1Factory* factory, HWND m_hwnd, D2D1_SIZE_U size)
{
    ID2D1HwndRenderTarget* pRenderTargetResult = NULL;
    HRESULT result = factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &pRenderTargetResult);

    if (FAILED(result))
    {
        throw - 1;
    }
    return std::unique_ptr<ID2D1HwndRenderTarget>(pRenderTargetResult);
}

std::unique_ptr<ID2D1SolidColorBrush> direct2d::create_brush(ID2D1HwndRenderTarget* renderTarget, D2D1::ColorF color)
{
    ID2D1SolidColorBrush* pBrushResult = NULL;
    HRESULT result = renderTarget->CreateSolidColorBrush(color, &pBrushResult);
    if (FAILED(result))
    {
        throw - 1;
    }
    return std::unique_ptr<ID2D1SolidColorBrush>(pBrushResult);
}
