#include "mainwindowlogic.h"

#include <ai/tableviewimpl.h>

MainWindowLogic::MainWindowLogic(const HWND m_hwnd) :
    m_hwnd{ m_hwnd },
    layout(35, 25),
    table(35, 25),
    IAmStartingTheGame{ true }
{}

auto MainWindowLogic::Create(const HWND m_hwnd) -> std::expected<std::unique_ptr<MainWindowLogic>, Win32Error>
{
    auto main_window_logic = std::unique_ptr<MainWindowLogic>(new MainWindowLogic(m_hwnd));
    direct2d::initialize_2d_brush_toolkit(&main_window_logic->brush_toolkit, m_hwnd);
    main_window_logic->CalculateLayout();
    return main_window_logic;
}

void MainWindowLogic::CalculateLayout()
{
    D2D1_SIZE_F size = brush_toolkit.pRenderTarget->GetSize();
    layout.ChangeSize(size.width, size.height);
}

void MainWindowLogic::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    brush_toolkit.pRenderTarget->BeginDraw();

    brush_toolkit.pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (size_t i = 0; i < layout.numberOfFields; i++) {

        D2D1_RECT_F rect = D2D1::RectF(layout.fields[i].positionX,
            layout.fields[i].positionY,
            layout.fields[i].positionX + layout.commonFieldStructure.size,
            layout.fields[i].positionY + layout.commonFieldStructure.size
        );
        brush_toolkit.pRenderTarget->FillRectangle(rect, brush_toolkit.borderBrush.get());

        D2D1_SIZE_F size = brush_toolkit.pRenderTarget->GetSize();
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
            selectedBackgroundBrushP = brush_toolkit.cursorSelectedBackgroundBrush.get();
        }
        else if (layout.fields[i].mouseOver) {
            selectedBackgroundBrushP = brush_toolkit.mouseOverBackgroundBrush.get();
        }
        else if (table.fields[i].lastMove) {
            selectedBackgroundBrushP = brush_toolkit.lastMoveBackgroundBrush.get();
        }
        else if (table.fields[i].highlight) {
            selectedBackgroundBrushP = brush_toolkit.highLightBackgroundBrush.get();
        }
        else {
            selectedBackgroundBrushP = brush_toolkit.backgroundBrush.get();
        }

        brush_toolkit.pRenderTarget->FillRectangle(rect, selectedBackgroundBrushP);

        if (table.fields[i].fieldState == FieldState::Naught) {
            const float x = layout.fields[i].positionX + (layout.commonFieldStructure.size / 2.0f);
            const float y = layout.fields[i].positionY + (layout.commonFieldStructure.size / 2.0f);
            const float radius = std::min(layout.commonFieldStructure.size / 2.0f, layout.commonFieldStructure.size / 2.0f);

            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
            brush_toolkit.pRenderTarget->FillEllipse(ellipse, brush_toolkit.player1Brush.get());
            ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius * 0.8f, radius * 0.8f);
            brush_toolkit.pRenderTarget->FillEllipse(ellipse, brush_toolkit.backgroundBrush.get());
        }
        else if (table.fields[i].fieldState == FieldState::Cross) {
            brush_toolkit.pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY),
                D2D1::Point2F(layout.fields[i].positionX + layout.commonFieldStructure.size, layout.fields[i].positionY + layout.commonFieldStructure.size),
                brush_toolkit.player2Brush.get(),
                1.5f
            );

            brush_toolkit.pRenderTarget->DrawLine(
                D2D1::Point2F(layout.fields[i].positionX, layout.fields[i].positionY + layout.commonFieldStructure.size),
                D2D1::Point2F(layout.fields[i].positionX + layout.commonFieldStructure.size, layout.fields[i].positionY),
                brush_toolkit.player2Brush.get(),
                border
            );
        }
    }


    HRESULT hr = brush_toolkit.pRenderTarget->EndDraw();
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

    brush_toolkit.pRenderTarget->Resize(size);
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

void MainWindowLogic::OnNew()
{
    table.reset();
    if (!IAmStartingTheGame) {
        handleOpponent();
    }
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindowLogic::OnIAmStartingTheGameChange(bool newValue)
{
    IAmStartingTheGame = newValue;
    if (table.getTableState() == TableState::AnybodyToPlace) {
        //Start a new game in case the table is untouched
        //triggering the AI player to place a mark
        OnNew();
    }
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
    if (table.getTableState() == TableState::NaughtWins) {
        MessageBox(m_hwnd, L"You won!", L"Report on game state change", MB_OK);
    }
    
    handleOpponent();
}

void MainWindowLogic::handleOpponent()
{
    TableViewImplementation tw(&table, FieldState::Cross);
    Position p2 = ai.calculateAnswer(&tw);
    table.mark(p2, FieldState::Cross);
    if (table.getTableState() == TableState::CrossWins) {
        MessageBox(m_hwnd, L"You lost!", L"Report on game state change", MB_OK);
    }
}
