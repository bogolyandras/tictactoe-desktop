#include "D2BrushToolkit.hpp"

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
        auto pFactory = create_factory();
        if (!pFactory.has_value())
        {
            return std::unexpected(pFactory.error());
        }
        brush_toolkit->pFactory.reset(*pFactory);

        auto size_e = get_size_of_window(m_hwnd);
        if (!size_e.has_value())
        {
            return std::unexpected(size_e.error());
        }
        const D2D1_SIZE_U size = size_e.value();

        auto pRenderTarget = create_render_target(*pFactory, m_hwnd, size);
        if (!pRenderTarget.has_value())
        {
            return std::unexpected(pRenderTarget.error());
        }
        brush_toolkit->pRenderTarget.reset(*pRenderTarget);

        auto backgroundBrush = create_brush(*pRenderTarget, D2D1::ColorF::White);
        if (!backgroundBrush.has_value())
        {
            return std::unexpected(backgroundBrush.error());
        }
        brush_toolkit->backgroundBrush.reset(*backgroundBrush);

        auto cursorSelectedBackgroundBrush = create_brush(*pRenderTarget, D2D1::ColorF::LightBlue);
        if (!cursorSelectedBackgroundBrush.has_value())
        {
            return std::unexpected(cursorSelectedBackgroundBrush.error());
        }
        brush_toolkit->cursorSelectedBackgroundBrush.reset(*cursorSelectedBackgroundBrush);

        auto mouseOverBackgroundBrush = create_brush(*pRenderTarget, D2D1::ColorF::LightGreen);
        if (!mouseOverBackgroundBrush.has_value())
        {
            return std::unexpected(mouseOverBackgroundBrush.error());
        }
        brush_toolkit->mouseOverBackgroundBrush.reset(*mouseOverBackgroundBrush);

        auto lastMoveBackgroundBrush = create_brush(*pRenderTarget, D2D1::ColorF::BlanchedAlmond);
        if (!lastMoveBackgroundBrush.has_value())
        {
            return std::unexpected(lastMoveBackgroundBrush.error());
        }
        brush_toolkit->lastMoveBackgroundBrush.reset(*lastMoveBackgroundBrush);

        auto highLightBackgroundBrush = create_brush(*pRenderTarget, D2D1::ColorF::Violet);
        if (!highLightBackgroundBrush.has_value())
        {
            return std::unexpected(highLightBackgroundBrush.error());
        }
        brush_toolkit->highLightBackgroundBrush.reset(*highLightBackgroundBrush);

        auto borderBrush = create_brush(*pRenderTarget, D2D1::ColorF::DarkOrange);
        if (!borderBrush.has_value())
        {
            return std::unexpected(borderBrush.error());
        }
        brush_toolkit->borderBrush.reset(*borderBrush);

        auto player1Brush = create_brush(*pRenderTarget, D2D1::ColorF::PaleVioletRed);
        if (!player1Brush.has_value())
        {
            return std::unexpected(player1Brush.error());
        }
        brush_toolkit->player1Brush.reset(*player1Brush);

        auto player2Brush = create_brush(*pRenderTarget, D2D1::ColorF::DarkGreen);
        if (!player2Brush.has_value())
        {
            return std::unexpected(player2Brush.error());
        }
        brush_toolkit->player2Brush.reset(*player2Brush);

        return {};
    }
}