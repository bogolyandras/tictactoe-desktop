#include "mainwindowlogic.h"

#include <ai/tableviewimpl.h>

MainWindowLogic::MainWindowLogic(HWND m_hwnd) :
    m_hwnd{ m_hwnd },
    layout(35, 25),
    table(35, 25),
    ai()
{
    pFactory = direct2d::create_factory();
    D2D1_SIZE_U size = direct2d::get_size_of_window(m_hwnd);
    pRenderTarget = direct2d::create_render_target(pFactory.get(), m_hwnd, size);
    backgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::White);
    cursorSelectedBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::LightBlue);
    mouseOverBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::LightGreen);
    lastMoveBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::BlanchedAlmond);
    highlighBackgroundBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::Violet);
    borderBrush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::DarkOrange);
    player1Brush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::PaleVioletRed);
    player2Brush = direct2d::create_brush(pRenderTarget.get(), D2D1::ColorF::DarkGreen);
    CalculateLayout();
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

    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (size_t i = 0; i < layout.numberOfFields; i++) {

        D2D1_RECT_F rect = D2D1::RectF(layout.fields[i].positionX,
            layout.fields[i].positionY,
            layout.fields[i].positionX + layout.commonFieldStructure.size,
            layout.fields[i].positionY + layout.commonFieldStructure.size
        );
        pRenderTarget->FillRectangle(rect, borderBrush.get());

        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float minSize = std::min(size.width, size.height);

        const float border = minSize / 400;
        const float halfBorder = minSize / 800;
        rect = D2D1::RectF(layout.fields[i].positionX + halfBorder,
            layout.fields[i].positionY + halfBorder,
            layout.fields[i].positionX + layout.commonFieldStructure.size - halfBorder,
            layout.fields[i].positionY + layout.commonFieldStructure.size - halfBorder
        );

        ID2D1SolidColorBrush* selectedBackgroundBrushP;
        if (layout.fields[i].cursorOver) {
            selectedBackgroundBrushP = cursorSelectedBackgroundBrush.get();
        }
        else if (layout.fields[i].mouseOver) {
            selectedBackgroundBrushP = mouseOverBackgroundBrush.get();
        }
        else if (table.fields[i].lastMove) {
            selectedBackgroundBrushP = lastMoveBackgroundBrush.get();
        }
        else if (table.fields[i].highlight) {
            selectedBackgroundBrushP = highlighBackgroundBrush.get();
        }
        else {
            selectedBackgroundBrushP = backgroundBrush.get();
        }

        pRenderTarget->FillRectangle(rect, selectedBackgroundBrushP);

        if (table.fields[i].fieldState == FieldState::Naught) {
            const float x = layout.fields[i].positionX + (layout.commonFieldStructure.size / 2.0f);
            const float y = layout.fields[i].positionY + (layout.commonFieldStructure.size / 2.0f);
            const float radius = std::min(layout.commonFieldStructure.size / 2.0f, layout.commonFieldStructure.size / 2.0f);

            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
            pRenderTarget->FillEllipse(ellipse, player1Brush.get());
            ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius * 0.8f, radius * 0.8f);
            pRenderTarget->FillEllipse(ellipse, backgroundBrush.get());
        }
        else if (table.fields[i].fieldState == FieldState::Cross) {
            pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY),
                D2D1::Point2F(layout.fields[i].positionX + layout.commonFieldStructure.size, layout.fields[i].positionY + layout.commonFieldStructure.size),
                player2Brush.get(),
                1.5f
            );

            pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY + layout.commonFieldStructure.size),
                D2D1::Point2F(layout.fields[i].positionX + layout.commonFieldStructure.size, layout.fields[i].positionY),
                player2Brush.get(),
                border
            );
        }
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

void MainWindowLogic::OnKeyPressUp()
{
    layout.OnKeyPressUp();
}

void MainWindowLogic::OnKeyPressDown()
{
    layout.OnKeyPressDown();
}

void MainWindowLogic::OnKeyPressLeft()
{
    layout.OnKeyPressLeft();
}

void MainWindowLogic::OnKeyPressRight()
{
    layout.OnKeyPressRight();
}

void MainWindowLogic::OnKeyPressPlace()
{
    handlePosition(layout.cursorPosition);
}

void MainWindowLogic::OnMouseMove(int positionX, int positionY)
{
    layout.OnMouseMove(positionX, positionY);
}

void MainWindowLogic::OnMouseClick(int positionX, int positionY)
{
    layout.OnMouseMove(positionX, positionY);
    Position p = layout.mousePosition();
    handlePosition(p);
}

void MainWindowLogic::handlePosition(Position p)
{
    const bool marked = table.mark(p, FieldState::Naught);
    if (!marked) {
        return;
    }
    TableViewImplementation tw(&table, FieldState::Cross);
    Position p2 = ai.calculateAnswer(&tw);
    table.mark(p2, FieldState::Cross);
}
