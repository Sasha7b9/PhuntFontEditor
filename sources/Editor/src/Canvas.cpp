#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportDialog.h"
#include "Font/BitmapFont.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


static Font font;

static wxCoord mouseX;
static wxCoord mouseY;


/// ѕодсветить клетку, на которую указывает мышь
static void HighlightCell(wxPaintDC &dc);
/// ƒействующий размер холста
static wxSize CurrentSize();


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    sw = static_cast<wxScrolledWindow *>(parent);

    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,        &Canvas::OnPaint,      this);
    Bind(wxEVT_MOTION,       &Canvas::OnMouseMove,  this);
    Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnMouseLeave, this);

    TuneScrollBar();

    Rebuild();
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(wxColor(0xF0, 0xF0, 0xF0)));

    dc.DrawRectangle(GetRect());

    font.Draw(dc);

    if (font.pixelsInPoint > 5)
    {
        dc.SetPen(wxPen(wxColor(0xa0, 0xa0, 0xa0)));

        for (int i = 0; i < GetSize().x; i += font.pixelsInPoint)
        {
            dc.DrawLine(i, 0, i, GetSize().y);
        }

        for (int i = 0; i < GetSize().y; i += font.pixelsInPoint)
        {
            dc.DrawLine(0, i, GetSize().x, i);
        }
    }

    dc.SetPen(*wxBLACK);
    
    for (int i = 0; i < GetSize().x; i += font.size.x * font.pixelsInPoint)
    {
        dc.DrawLine(i, 0, i, GetSize().y);
    }
    
    for (int i = 0; i < GetSize().y; i += font.size.y * font.pixelsInPoint)
    {
        dc.DrawLine(0, i, GetSize().x, i);
    }
    
    HighlightCell(dc);
}


static void HighlightCell(wxPaintDC &dc)
{
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    int x = (mouseX / (font.pixelsInPoint)) * (font.pixelsInPoint);

    int y = (mouseY / (font.pixelsInPoint)) * (font.pixelsInPoint);

    dc.DrawRectangle(x, y, font.pixelsInPoint, font.pixelsInPoint);
}


void Canvas::OnMouseMove(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    Refresh();
}


void Canvas::Increase()
{
    if (font.pixelsInPoint < 32)
    {
        font.pixelsInPoint++;

        Resize();

        font.Resize();

        TuneScrollBar();

        Refresh();
    }
}


void Canvas::Decrease()
{
    if (font.pixelsInPoint > 2)
    {
        font.pixelsInPoint--;

        Resize();

        font.Resize();

        TuneScrollBar();

        Refresh();
    }
}


void Canvas::OnMouseLeave(wxMouseEvent &)
{
    mouseX = mouseY = -100;
    Refresh();
}


static wxSize CurrentSize()
{
    return { 16 * font.size.x * font.pixelsInPoint + 1, 16 * font.size.y * font.pixelsInPoint + 1 };
}


void Canvas::TuneScrollBar()
{
    wxSize size = CurrentSize();

    sw->SetScrollbars(1, 1, size.x, size.y);

    SetPosition({ 0, 0 });
}


void Canvas::OnMouseRightDown(wxMouseEvent &)
{

}


void Canvas::OnMouseRightUp(wxMouseEvent &)
{

}


void Canvas::Rebuild()
{
    font.CreateNew();

    SetSize(CurrentSize());

    Refresh();
}


void Canvas::Resize()
{
    SetSize(CurrentSize());
    font.Resize();
}


void Canvas::ClearBadSymbols()
{
    font.ClearBadSymbols();
}
