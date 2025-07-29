#include "D2BrushToolkit.hpp"

#define TRY_TO_ASSIGN(target, expression) \
    do { \
        auto result = (expression); \
        if (!result.has_value()) { \
            return std::unexpected(result.error()); \
        } \
        target.reset(*result); \
    } while(0)


namespace direct2d
{
    auto create_factory() -> std::expected<ID2D1Factory*, Win32Error>
    {
        ID2D1Factory* pFactoryResult = nullptr;
        if (const HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactoryResult); FAILED(result))
        {
            return std::unexpected(Win32Error::fromLastError());
        }
        return pFactoryResult;
    }

    auto get_size_of_window(HWND m_hwnd) -> std::expected<D2D1_SIZE_U, Win32Error>
    {
        RECT rc;
        if (!GetClientRect(m_hwnd, &rc))
        {
            return std::unexpected(Win32Error::fromLastError());
        }
        return D2D1::SizeU(rc.right, rc.bottom);
    }

    auto create_render_target(ID2D1Factory* factory, const HWND m_hwnd, const D2D1_SIZE_U size) -> std::expected<ID2D1HwndRenderTarget*, Win32Error>
    {
        ID2D1HwndRenderTarget* pRenderTargetResult = nullptr;
        const HRESULT result = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTargetResult);

        if (FAILED(result))
        {
            return Win32Error::fromLastError();
        }
        return pRenderTargetResult;
    }

    auto create_brush(ID2D1HwndRenderTarget* renderTarget, D2D1::ColorF color) -> std::expected<ID2D1SolidColorBrush*, Win32Error>
    {
        ID2D1SolidColorBrush* pBrushResult = nullptr;
        if (const HRESULT result = renderTarget->CreateSolidColorBrush(color, &pBrushResult); FAILED(result))
        {
            return Win32Error::fromLastError();
        }
        return pBrushResult;
    }

    auto initialize_2d_brush_toolkit(D2BrushToolkit* brush_toolkit, HWND m_hwnd) -> std::expected<void, Win32Error>
    {
        TRY_TO_ASSIGN(brush_toolkit->pFactory,
            create_factory());

        auto size_e = get_size_of_window(m_hwnd);
        if (!size_e.has_value())
        {
            return std::unexpected(size_e.error());
        }
        const D2D1_SIZE_U size = size_e.value();

        TRY_TO_ASSIGN(brush_toolkit->pRenderTarget,
            create_render_target(brush_toolkit->pFactory.get(), m_hwnd, size));

        const auto pRenderTarget = brush_toolkit->pRenderTarget.get();

        TRY_TO_ASSIGN(brush_toolkit->backgroundBrush,
            create_brush(pRenderTarget, D2D1::ColorF::White));

        TRY_TO_ASSIGN(brush_toolkit->cursorSelectedBackgroundBrush,
            create_brush(pRenderTarget, D2D1::ColorF::LightBlue));

        TRY_TO_ASSIGN(brush_toolkit->mouseOverBackgroundBrush,
            create_brush(pRenderTarget, D2D1::ColorF::LightGreen));

        TRY_TO_ASSIGN(brush_toolkit->lastMoveBackgroundBrush,
            create_brush(pRenderTarget, D2D1::ColorF::BlanchedAlmond));

        TRY_TO_ASSIGN(brush_toolkit->highLightBackgroundBrush,
            create_brush(pRenderTarget, D2D1::ColorF::Violet));

        TRY_TO_ASSIGN(brush_toolkit->borderBrush,
            create_brush(pRenderTarget, D2D1::ColorF::DarkOrange));

        TRY_TO_ASSIGN(brush_toolkit->player1Brush,
            create_brush(pRenderTarget, D2D1::ColorF::PaleVioletRed));

        TRY_TO_ASSIGN(brush_toolkit->player2Brush,
            create_brush(pRenderTarget, D2D1::ColorF::DarkGreen));

        return {};
    }
}